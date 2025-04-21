#include <stack>
#include <iostream>
#include <memory>


#include "FlexLexer.h"


#include "include/reader.hh"
#include "parser.hh"
#include "lexer.hh"


int main(int argc, char **argv) {
  std::unique_ptr<utils::Reader> reader;
  std::istream* input = nullptr;
  try {
    if (argc > 1) {
      reader = std::make_unique<utils::Reader>(argv[1]);
    } else {
      reader = std::make_unique<utils::Reader>("");
    }
    input = &reader->stream();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  Lexer lexer(input);
  yy::parser parser(lexer);
  parser.parse();
  return 0;
}