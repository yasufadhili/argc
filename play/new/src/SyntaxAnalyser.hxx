#pragma once

#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Registry.hxx"
#include "TokenType.hxx"
#include "Token.hxx"
#include "SourceLocation.hxx"


class SyntaxError final : public std::runtime_error {
public:
  explicit SyntaxError(const std::string &msg): runtime_error(msg) {
    throw std::runtime_error("SyntaxError: " + msg);
  }
};


class SyntaxAnalyser {

  size_t index_{0};

  std::vector<Token> tokens_;

  std::vector<Token> tokens_out_;

  std::stack<TokenType> delimiter_stack_;

  std::unordered_map<TokenType, TokenType> delimiter_pairs_ {
    {TokenType::LPAREN, TokenType::RPAREN},
    {TokenType::LBRACE, TokenType::RBRACE},
    {TokenType::LBRACKET, TokenType::RBRACKET}
  };

  auto throw_error(const std::string &msg) const -> void;
  auto throw_error(const std::string &msg, const std::string &lexeme) const -> void;

  [[nodiscard]] auto is_at_end() const -> bool;
  auto peek(const size_t &offset) -> Token;
  auto advance() -> Token;
  [[nodiscard]] auto match(const TokenType&) -> bool;
  [[nodiscard]] auto match(const std::vector<TokenType>&) -> bool;


  auto let() -> void;
  auto exit() -> void;
  auto set() -> void;
  auto number() -> void;
  auto identifier() -> void;
  auto type_identifier() -> void;
  auto delimiter() -> void;
  auto single_char() -> void;
  auto double_char() -> void;

  auto validate() -> void;


public:
  explicit SyntaxAnalyser(const std::vector<Token> &tokens) : tokens_(tokens) {}

  auto analyse() -> std::vector<Token>;

};

inline auto SyntaxAnalyser::throw_error(const std::string &msg) const -> void {
  throw SyntaxError(
    msg + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto SyntaxAnalyser::throw_error(const std::string &msg, const std::string &lexeme) const -> void {
  throw SyntaxError(
    msg + " '" + lexeme + "' " + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto SyntaxAnalyser::is_at_end() const -> bool {
  return tokens_.at(index_).get_type() == TokenType::END_OF_FILE;
}

inline auto SyntaxAnalyser::peek(const size_t &offset = 0) -> Token {
  size_t idx = index_ + offset;
  if (idx >= tokens_.size()) {
    // Return EOF token if we're past the end
    return Token(TokenType::END_OF_FILE, "", SourceLocation(0, 0, -1));
  }
  return tokens_.at(idx);
}

inline auto SyntaxAnalyser::advance() -> Token {
  if (is_at_end()) {
    return tokens_.at(index_);
  }
  index_++;
  return tokens_.at(index_);
}

inline auto SyntaxAnalyser::match(const TokenType &type) -> bool {
  return peek().get_type() == type;
}

inline auto SyntaxAnalyser::match(const std::vector<TokenType> &types) -> bool {
  for (const auto& type : types) {
    if (peek().get_type() == type) {
      return true;
    }
  }
  return false;
}

inline auto SyntaxAnalyser::delimiter() -> void {

}

inline auto SyntaxAnalyser::let() -> void {
  // let <ident> : <type>; or let <ident> : <type> = ...
  if (
    peek(1).get_type() == TokenType::IDENTIFIER
    && peek(2).get_type() == TokenType::COLON
    && peek(3).get_type() == TokenType::TYPE_IDENTIFIER
    && ( peek(4).get_type() == TokenType::SEMICOLON || peek(4).get_type() == TokenType::EQUALS )
    ) {
    return;
  }
  throw_error("Improper use of 'let' keyword");
}

inline auto SyntaxAnalyser::set() -> void {
  if (peek(-1).get_type() != TokenType::EQUALS) {
    throw_error("Expected = before 'set'");
  }
  if (peek(1).get_type() != TokenType::LPAREN) {
    throw_error("Expected '('");
  }
}

inline auto SyntaxAnalyser::exit() -> void {

}

inline auto SyntaxAnalyser::number() -> void {

}

inline auto SyntaxAnalyser::type_identifier() -> void {

}




inline auto SyntaxAnalyser::identifier() -> void {

}

inline auto SyntaxAnalyser::single_char() -> void {
  switch (peek().get_type()) {
    case TokenType::EQUALS:
      if (peek(-1).get_type() != TokenType::TYPE_IDENTIFIER) {
        throw_error("Invalid use of = operator");
      }
      break;
    case TokenType::COLON:
      if (peek(-1).get_type() != TokenType::IDENTIFIER) {
        throw_error("Invalid use of : operator");
      } else if (peek(1).get_type() != TokenType::TYPE_IDENTIFIER) {
        throw_error("Invalid use of : operator");
      }
      break;
    case TokenType::SEMICOLON:
      break;
    default:
      throw_error("Invalid use of operator", peek().get_lexeme());
  }
}

inline auto SyntaxAnalyser::validate() -> void {
  switch (peek().get_type()) {

    case TokenType::LET:
      let();
    break;

    case TokenType::IDENTIFIER:
      identifier();
    break;

    case TokenType::SET:
      set();
    break;

    case TokenType::EXIT:
      exit();
      break;

    case TokenType::TYPE_IDENTIFIER:
      type_identifier();
      break;

    case TokenType::INTEGER:
      number();
      break;

    default:
      if (CharRegistry::get_instance().is_delimiter(peek().get_type())) {
        delimiter();
        break;
      } else if (CharRegistry::get_instance().is_single_char(peek().get_type())) {
        single_char();
        break;
      }
    throw_error("Unidentified token", peek().get_lexeme());

  }
}


inline auto SyntaxAnalyser::analyse() -> std::vector<Token> {
  while (!is_at_end()) {
    validate();
    advance();
  }
  return tokens_;
}






