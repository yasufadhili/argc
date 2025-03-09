

#include "SourceFileHandler.hxx"
#include "Lexer.hxx"
#include "SyntaxAnalyser.hxx"
#include "Parser.hxx"


auto main(int argc, char* argv[]) -> int {

  if (argc != 2) {
    std::cerr << "Usage: " << "New" << " <filename>" << std::endl;
    return EXIT_FAILURE;
  }

  SourceFileHandler src_file_handler(argv[1]);

  std::string content = src_file_handler.read_to_string();

  src_file_handler.close();

  Lexer lexer(content);

  //auto tokens = lexer.lex();

  //for (auto &token : tokens) {
  //  std::cout << token.to_string() << std::endl;
  //}

  SyntaxAnalyser syntax_analyser(lexer.lex());

  //syntax_analyser.analyse();

  Parser parser(syntax_analyser.analyse());

  auto ast = parser.parse();

  //system("nasm -f elf64 out.asm");


  return 0;
}
