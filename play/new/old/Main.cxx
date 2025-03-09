
#include <iostream>

#include "SourceFileHandler.hxx"
#include "Lexer.hxx"

auto main() -> int {

  SourceFileHandler src_file_handler("../examples/source.ar");

  std::string content = src_file_handler.read_to_string();

  src_file_handler.close();

  Lexer lexer(content);

  auto tokens = lexer.lex();

  for (auto &token : tokens) {
    std::cout << token.get_lexeme() << std::endl;
  }


  return 0;
}