#include <string>

#include "FileHandler.hxx"
#include "Lexer.hxx"
#include "Parser.hxx"


auto main() -> int {

  std::string source_file {"../examples/1_return.ar"};

  std::string src = read_file_to_string(source_file);

  Lexer lexer {src};
  Parser parser {lexer.lex()};

}
