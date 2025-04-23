
#include <cstdlib>
#include <fstream>
#include <memory>
#include <iostream>
#include <string>

#include "lexer.hh"
#include "parser.hh"
#include "include/ast.hh"


auto main(const int argc, char* argv[]) -> int {
  std::fstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Unable to open file" << std::endl;
    exit(EXIT_FAILURE);
  }

  yy::Lexer lexer{input, std::cerr};
  lexer.set_debug(false);

  std::shared_ptr<ast::prog::Program> program;

  yy::Parser parser{lexer, program};
  parser.set_debug_level(0);

  if (parser.parse() != 0) {
    std::cerr << "Parsing failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  program->print(0);

  return 0;
}