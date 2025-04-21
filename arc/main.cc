
#include <cstdlib>

#include "lexer.hh"
#include "parser.hh"


auto main() -> int {

  yy::Lexer lexer{std::cin, std::cout};
  lexer.set_debug(false);

  yy::Parser parser{lexer};
  parser.set_debug_level(0);

  std::cout << "> ";
  return parser();
}