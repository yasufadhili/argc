
#include <iostream>

#include "FileHandler.hxx"
#include "Lexer.hxx"
#include "Analyser.hxx"
#include "Parser.hxx"


auto main(int argc, char* argv[] ) -> int {

  std::string out_name {"out"};

  if (argc != 2) {
    std::cerr << "Usage: " << "argon" << " <file>" << std::endl;
    return 1;
  }

  FileHandler file_handler(argv[1]);
  std::string src = file_handler.read_to_string();

  Lexer lexer(src);

  Analyser analyser(lexer.lex());

  Parser parser(analyser.analyse());

  auto ast = parser.parse();

  ast::Visitor visitor;
  ast->accept(visitor);

  std::ofstream out_file("out.asm");
  if (out_file.is_open()) {
    out_file << visitor.get_output();
    out_file.close();

    system("nasm -f elf64 out.asm");
    system("ld -o out out.o");

    //system("rm out.asm out.o");

  } else {
    std::cerr << "Could not write to output file." << std::endl;
  }

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
  std::cerr << "Please note that the created executable will not be able to run on this computer's CPU architecture" << std::endl;
#endif

#ifdef __x86_64__

#else

  std::cerr << "Please note that the created executable will not be able to run on this computer's CPU architecture" << std::endl;

#endif

  return 0;
}
