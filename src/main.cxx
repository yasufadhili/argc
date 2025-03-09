#include <cstdlib>


#include "ast.hxx"
#include "parser.hxx"
#include "token.hxx"
#include "file_utils.hxx"


auto generate_tokens() -> std::vector<token::Token> {
  using namespace token;
  const SourceLocation sl("", 0, 0, 0);
  std::vector<Token> tokens{
    {TokenType::KEYWORD, KeywordType::LET, "let", sl},
    {TokenType::IDENTIFIER, "_start", "_start", sl},
    {TokenType::PUNCTUATOR, PunctuatorType::COLON, ":", sl},
    {TokenType::IDENTIFIER, IdentifierType::TYPE, "i8", sl},
    {TokenType::OPERATOR, OperatorType::ASSIGN, "=", sl},
    {TokenType::KEYWORD, KeywordType::SET, "set", sl},
    {TokenType::DELIMITER, DelimiterType::LPAREN, "(", sl},
    {TokenType::DELIMITER, DelimiterType::RPAREN, "set", sl},
    {TokenType::DELIMITER, DelimiterType::LBRACE, "{", sl},
{TokenType::KEYWORD, KeywordType::EXIT, "exit", sl},
{TokenType::NUMBER, NumberType::INTEGER, "30", sl},
{TokenType::PUNCTUATOR, PunctuatorType::SEMICOLON, ";", sl},
    {TokenType::DELIMITER, DelimiterType::RBRACE, "}", sl},
    {TokenType::END_OF_FILE, "", "", sl}
  };
  return tokens;
}


auto main() -> int {
  parser::Parser parser(generate_tokens());

  auto ast = parser.parse();

  ast::Visitor visitor;

  ast->accept(visitor);


  if (std::ofstream out_file("out.asm"); out_file.is_open()) {
    out_file << visitor.get_output();
    out_file.close();

  } else {
    fprintf(stderr, "Could not write to output file.");
    exit(1);
  }

  system("nasm -f elf64 out.asm");
  system("ld -o out out.o");

  system("rm out.asm out.o");


  return EXIT_SUCCESS;
}
