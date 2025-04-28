#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "analyser.hh"
#include "config.hh"
#include "util_logger.hh"
#include "lexer.hh"
#include "parser.hh"
#include "ast.hh"
#include "driver.hh"

namespace fs = std::filesystem;

// Execute external process safely
bool execute_process(const std::vector<std::string>& args, std::string& error_output) {
  // Prepare arguments for process execution
  std::vector<const char*> c_args;
  for (const auto& arg : args) {
    c_args.push_back(arg.c_str());
  }
  c_args.push_back(nullptr);  // Null terminator

  std::array<int, 2> stderr_pipe;
  if (pipe(stderr_pipe.data()) != 0) {
    error_output = "Failed to create pipe for stderr";
    return false;
  }

  pid_t child_pid = fork();
  if (child_pid == -1) {
    error_output = "Failed to fork process";
    close(stderr_pipe[0]);
    close(stderr_pipe[1]);
    return false;
  }

  if (child_pid == 0) {
    // Child process
    close(stderr_pipe[0]);  // Close read end
    dup2(stderr_pipe[1], STDERR_FILENO);
    close(stderr_pipe[1]);

    execvp(c_args[0], const_cast<char* const*>(c_args.data()));
    
    // If we get here, exec failed
    std::string err_msg = "Failed to execute: " + args[0];
    write(STDERR_FILENO, err_msg.c_str(), err_msg.length());
    _exit(EXIT_FAILURE);
  }

  // Parent process
  close(stderr_pipe[1]);  // Close write end

  // Read error output
  char buffer[4096];
  error_output.clear();
  ssize_t bytes_read;
  while ((bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer) - 1)) > 0) {
    buffer[bytes_read] = '\0';
    error_output += buffer;
  }
  close(stderr_pipe[0]);

  // Wait for child process
  int status;
  waitpid(child_pid, &status, 0);

  return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

// Handles assembly and linking
bool build_executable (
  const fs::path& asm_path, 
  const fs::path& obj_path,
  const fs::path& output_path,
  logger::Logger& logger
  ) {
  std::string error_output;
  
  // Assemble
  std::vector<std::string> assemble_args = {
    "as",
    "-o", obj_path.string(),
    asm_path.string()
  };
  
  logger.log(logger::LogLevel::DEBUG, "Running assembler: as -o " + obj_path.string() + " " + asm_path.string());
              
  if (!execute_process(assemble_args, error_output)) {
    logger.log(logger::LogLevel::ERROR, "Assembly failed: " + error_output);
    return false;
  }
  
  // Link
  std::vector<std::string> link_args = {
    "ld",
    "-o", output_path.string(),
    obj_path.string()
  };
  
  logger.log(logger::LogLevel::DEBUG, "Running linker: ld -o " + output_path.string() + " " + obj_path.string());
              
  if (!execute_process(link_args, error_output)) {
    logger.log(logger::LogLevel::ERROR, "Linking failed: " + error_output);
    return false;
  }
  
  return true;
}

auto main(const int argc, char* argv[]) -> int {


  auto& logger { logger::Logger::instance() };
  logger.set_level(logger::LogLevel::TRACE);

  // Add console sink
  auto console_sink { std::make_shared<::logger::ConsoleSink>() };
  console_sink->set_level(logger::LogLevel::DEBUG); // Only show debug and above
  logger.add_sink(console_sink);

  try {

    const driver::OptionParser option_parser(argc, argv);

    config::Config config;

    if (!option_parser.parse(config)) {
      return 1;
    }

    if (config.show_help) {
      driver::display_help(argv[0]);
      return 0;
    }

    if (config.show_version) {
      driver::display_version();
      return 0;
    }

    if (!driver::validate_input_files(config)) {
      return 1;
    }

    // Convert input file to absolute path
    fs::path input_path = fs::absolute(config.input_files.at(0));
    std::ifstream in(input_path);
    
    if (!in.is_open()) {
      std::cerr << "Unable to open file '" << input_path << "'" << std::endl;
      return 1;
    }

    yy::Lexer lexer{in, std::cerr};
    lexer.set_debug(false);

    // Initialise the lexer with the first file
    if (!lexer.enter_file(input_path.string())) {
      std::cerr << "Failed to initialise lexer with input file" << std::endl;
      return 1;
    }

    std::shared_ptr<ast::prog::Program> program;

    yy::Parser parser{lexer, program};
    parser.set_debug_level(0);

    if (parser.parse() != 0) {
      std::cerr << "Parsing failed" << std::endl;
      return 1;
    }

    if (config.verbose){
      program->print(0);
    }

    analyser::SemanticAnalyser analyser;
    analyser.visit(program);

    bool s { analyser.analyse(*program) };
    
    if (s) {
      logger.log(logger::LogLevel::INFO, "Success analyse");
    } else {
      logger.log(logger::LogLevel::ERROR, "Failed analyse");
    }

    ast::Visitor visitor;
    visitor.visit(program);

    auto output = std::move(visitor.get_output());

    fs::path output_path = fs::absolute(config.input_files.at(0)).replace_extension(".asm");
    std::ofstream output_file { output_path.string() };
    if (!output_file.is_open()) {
      logger.log(logger::LogLevel::ERROR, "Failed to open output file");
      return 1;
    }

    output_file << output.str();
    output_file.close();

    fs::path obj_path = fs::absolute(config.input_files.at(0)).replace_extension(".o");
    fs::path exec_path = config.output_file.empty() ? 
                        input_path.parent_path() / input_path.stem() : 
                        fs::absolute(config.output_file);
    
    if (!build_executable(output_path, obj_path, exec_path, logger)) {
      return 1;
    }

    logger.log(logger::LogLevel::INFO, "Successfully built executable: " + exec_path.string());
    return 0;

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

}
