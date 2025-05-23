#include <ArgonParser.h>
#include "ArgonLexer.h"
#include <string>
#include <fstream>
#include <fmt/core.h>

#include "AstBuilder.hh"
#include "fmt/color.h"

auto main(const int argc, char *argv[]) -> int {

  if (argc < 2) {
    fmt::print(stderr, "Usage: {} <input-file>\n", argv[0]);
    return 1;
  }

  std::string input_file_path = argv[1];
  std::ifstream input_file(input_file_path);
  if (!input_file.is_open()) {
    fmt::print(stderr, fg(fmt::color::crimson), "Error: ");
    fmt::print(stderr, "Could not open input file: {}\n", input_file_path);
    return 1;
  }

  std::string input_text((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
  input_file.close();

  antlr4::ANTLRInputStream input(input_text);
  ArgonLexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);
  ArgonParser parser(&tokens);

  ArgonParser::ModuleDeclarationContext* tree { parser.moduleDeclaration() };

  return 0;
}
