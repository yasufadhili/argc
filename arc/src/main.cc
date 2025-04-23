
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "lexer.hh"
#include "parser.hh"
#include "include/ast.hh"
#include "include/driver.hh"


auto main(const int argc, char* argv[]) -> int {

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

    std::fstream in(config.input_files.at(0));
    if (!in.is_open()) {
      std::cerr << "Unable to open file '" << config.input_files.at(0) << "'" << std::endl;
      return 1;
    }
    yy::Lexer lexer{in, std::cerr};
    lexer.set_debug(false);

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

  /**







  return 0;

  **/
}
