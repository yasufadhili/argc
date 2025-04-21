
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "lexer.hh"
#include "parser.hh"


auto main(const int argc, char* argv[]) -> int {

  std::fstream input("input.txt");
  if (!input.is_open()) {
    std::cerr << "Unable to open file" << std::endl;
    exit(EXIT_FAILURE);
  }

  yy::Lexer lexer{input, std::cerr};
  lexer.set_debug(false);

  yy::Parser parser{lexer};
  parser.set_debug_level(0);

  std::cout << "> ";
  return parser();
}