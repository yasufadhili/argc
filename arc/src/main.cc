#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>

#include "include/util_logger.hh"
#include "lexer.hh"
#include "parser.hh"
#include "include/ast.hh"
#include "include/driver.hh"

namespace fs = std::filesystem;

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

    // Initialize the lexer with the first file
    if (!lexer.enter_file(input_path.string())) {
      std::cerr << "Failed to initialize lexer with input file" << std::endl;
      return 1;
    }

    std::shared_ptr<ast::prog::Program> program;

    yy::Parser parser{lexer, program};
    parser.set_debug_level(0);

    if (parser.parse() != 0) {
      std::cerr << "Parsing failed" << std::endl;
      return 1;
    }

    program->print(0);

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

}
