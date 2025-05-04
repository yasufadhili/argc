#include <filesystem>
#include <fstream>
#include <llvm-17/llvm/Support/raw_ostream.h>
#include <llvm-17/llvm/IR/LLVMContext.h>
#include "ast.hh"
#include "lexer.hh"
#include "include/driver.hh"
#include "include/util_logger.hh"
#include "include/error_handler.hh"
#include "x86_64_codegen.hh"

namespace fs = std::filesystem;

auto main(const int argc, char* argv[]) -> int {
  
  auto& logger { logger::Logger::instance() };
  const auto console_sink = std::make_shared<logger::ConsoleSink>();
  logger.add_sink(console_sink);
  logger.set_level(logger::LogLevel::TRACE);

  const driver::OptionParser option_parser(argc, argv);
  config::Config config;

  if (!option_parser.parse(config)) {
    LOG_ERROR("Failed to parse config");
    return EXIT_FAILURE;
  }

  if (config.show_help) {
    driver::display_help(argv[0]);
    return EXIT_SUCCESS;
  }

  if (config.show_version) {
    driver::display_version();
    return EXIT_SUCCESS;
  }

  if (!driver::validate_input_files(config)) {
    LOG_ERROR("Failed to validate input files");
    return EXIT_FAILURE;
  }

  // Convert the input file to an absolute path
  fs::path input_path = fs::absolute(config.input_files.at(0));
  std::ifstream in(input_path);

  if (!in.is_open()) {
    LOG_ERROR("Unable to open file '" + input_path.string() + "'");
    return EXIT_FAILURE;
  }

  yy::Lexer lexer{ in, std::cerr };
  if (config.debug) {
    lexer.set_debug(true);
  }

  if (!lexer.enter_file(input_path.string())) {
    LOG_ERROR("Failed to initialise lexer input file");
    return EXIT_FAILURE;
  }

  std::shared_ptr<ast::unit::TranslationUnit> translation_unit;

  yy::Parser parser { lexer, translation_unit };
  if (config.debug) {
    parser.set_debug_level(1);
  }

  if (parser.parse() != 0) {
    LOG_FATAL("Failed to parse translation unit");
    return EXIT_FAILURE;
  }

  if (!translation_unit) {
    LOG_FATAL("Translation unit is null after parsing");
    return EXIT_FAILURE;
  }

  if (config.verbose) {
    LOG_INFO("Code Structure: ");
    try {
      ast::Printer printer;
      translation_unit->accept(printer);
    } catch (const std::exception& e) {
      LOG_ERROR("Error during printing: " + std::string(e.what()));
      return EXIT_FAILURE;
    }
  }
                                                    
  ast::SymbolCollector symbol_collector;
  translation_unit->accept(symbol_collector);
  if (!symbol_collector.successful()) {
    error::DiagnosticHandler::instance().print_all();
    std::cout << std::endl;
    std::cout << "Compilation failed during symbol collection with "
      << error::DiagnosticHandler::instance().message_count() << " messages: "
      << error::DiagnosticHandler::instance().error_count() << " errors, "
      << error::DiagnosticHandler::instance().warning_count() << " warnings"
      << "\n" << std::endl;
    return EXIT_FAILURE;
  }

  ast::SemanticAnalyser semantic_analyser;
  translation_unit->accept(semantic_analyser);

  if (error::DiagnosticHandler::instance().has_errors()) {
    error::DiagnosticHandler::instance().print_all();
    std::cout << std::endl;
    std::cout << "Compilation failed with "
    << error::DiagnosticHandler::instance().message_count() << " messages: "
    << error::DiagnosticHandler::instance().error_count() << " errors, "
    << error::DiagnosticHandler::instance().warning_count() << " warnings"
    << "\n" << std::endl;
    return EXIT_FAILURE;
  }

  //llvm::LLVMContext context;
  //ast::CodeGenerator codegen(context);
  //translation_unit->accept(codegen);
  //auto prog_module { codegen.take_module() };
  //prog_module->print(llvm::outs(), nullptr);

  ast::x86_64_CodeGenerator codegen;
  translation_unit->accept(codegen);

  fs::path output_path = fs::absolute(config.input_files.at(0)).replace_extension(".asm");
  std::ofstream output_file { output_path.string() };
  if (!output_file.is_open()) {
    LOG_ERROR("Failed to open file '" + output_path.string() + "'");
    return EXIT_FAILURE;
  }

  output_file << codegen.get_asm_code();
  output_file.close();

  return EXIT_SUCCESS; // Temporary till we get lexing, parsing, analysis working

  /**
  ast::CodeGenerator code_generator;
  code_generator.generate(translation_unit);

  fs::path output_path = fs::absolute(config.input_files.at(0)).replace_extension(".asm");
  std::ofstream output_file { output_path.string() };
  if (!output_file.is_open()) {
    LOG_ERROR("Failed to open file '" + output_path.string() + "'");
    return EXIT_FAILURE;
  }

  output_file << code_generator.get_output().str();
  output_file.close();

  if (config.verbose){
    LOG_INFO("Written to output file");
  }
  **/
  return 0;
}
