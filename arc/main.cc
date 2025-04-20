
#include <iostream>

#include "parser.hh"

auto main() -> int {

  yy::parser parser;

  std::cout << "Argon Compiler" << std::endl;

  auto res { parser.parse() };

  std::cout << res << std::endl;

  return 0;
}