#pragma once

#include <stack>
#include <stdexcept>
#include <vector>

#include "Token.hxx"



class SyntaxError final : public std::runtime_error {
public:
  explicit SyntaxError(const std::string &msg): runtime_error(msg) {
    throw std::runtime_error("SyntaxError: " + msg);
  }
};


class Analyser {

  size_t index_{0};
  std::vector<Token> tokens_;
  std::vector<Token> tokens_out_;


  auto throw_error(const std::string &msg) const -> void;
  auto throw_error(const std::string &msg, const std::string &lexeme) const -> void;

  [[nodiscard]] auto is_at_end() const -> bool;
  auto peek(const size_t &offset) -> Token;
  auto advance() -> Token;
  [[nodiscard]] auto match(const TokenType&) -> bool;
  [[nodiscard]] auto match(const std::vector<TokenType>&) -> bool;

  auto add_token(const Token&) -> void;
  auto add_token(const TokenType&) -> void;

  auto keyword() -> void;
  auto single_char() -> void;

  auto exit() -> void;
  auto let() -> void;
  auto set() -> void;
  auto integer() -> void;
  auto type_identifier() -> void;
  auto identifier() -> void;

  auto validate() -> void;


public:

  explicit Analyser(const std::vector<Token>& tokens);
  auto analyse() -> std::vector<Token>;

};

inline Analyser::Analyser(const std::vector<Token> &tokens) : tokens_(tokens) {}

inline auto Analyser::throw_error(const std::string &msg) const -> void {
  throw SyntaxError(
    msg + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto Analyser::throw_error(const std::string &msg, const std::string &lexeme) const -> void {
  throw SyntaxError(
    msg + " '" + lexeme + "' " + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto Analyser::is_at_end() const -> bool {
  return tokens_.at(index_).get_type() == TokenType::END_OF_FILE;
}

inline auto Analyser::peek(const size_t &offset = 0) -> Token {
  size_t idx = index_ + offset;
  if (idx >= tokens_.size()) {
    // Return EOF token if we're past the end
    return Token(TokenType::END_OF_FILE, "", SourceLocation(0, 0, -1));
  }
  return tokens_.at(idx);
}

inline auto Analyser::advance() -> Token {
  if (is_at_end()) {
    return tokens_.at(index_);
  }
  index_++;
  return tokens_.at(index_);
}

inline auto Analyser::match(const TokenType &type) -> bool {
  return peek().get_type() == type;
}

inline auto Analyser::match(const std::vector<TokenType> &types) -> bool {
  for (const auto& type : types) {
    if (peek().get_type() == type) {
      return true;
    }
  }
  return false;
}

inline auto Analyser::add_token(const Token &token) -> void {
  tokens_out_.emplace_back(token);
}

// Added this just to add the EOF in the tokens_out_ . SourceLocation isn't useful here
inline auto Analyser::add_token(const TokenType &) -> void {
  tokens_out_.emplace_back(TokenType::END_OF_FILE, "", SourceLocation(0, 0, -1));
}

inline auto Analyser::single_char() -> void {
  add_token(peek());
}

inline auto Analyser::exit() -> void {
  if (peek(1).get_type() != TokenType::INTEGER) {
    throw_error("Expected integer to exit with");
    return;
  }
  if (peek(2).get_type() != TokenType::SEMICOLON) {
    throw_error("Expected semicolon at end of exit statement");
    return;
  }
  add_token(peek());
  //advance();
}

inline auto Analyser::integer() -> void {
  add_token(peek());
}

inline auto Analyser::let() -> void {
  if (match({
    TokenType::LET,
    TokenType::IDENTIFIER,
    TokenType::COLON,
    TokenType::TYPE_IDENTIFIER,
    TokenType::SEMICOLON}))
    {
    add_token(peek());
    return;
  }
  if (match({
    TokenType::LET,
    TokenType::IDENTIFIER,
    TokenType::COLON,
    TokenType::TYPE_IDENTIFIER,
    TokenType::EQUALS,
  })) {
    add_token(peek());
    return;
  }
  throw_error("Invalid let statement");
}

inline auto Analyser::set() -> void {

}



inline auto Analyser::keyword() -> void {
  switch (TokenType type = peek().get_type()) {
    case TokenType::EXIT:
      exit();
      break;
    case TokenType::LET:
      let();
      break;
    case TokenType::SET:
      set();
      break;
    default:
      throw_error("Unidentified keyword", peek().get_lexeme());
  }
}

inline auto Analyser::type_identifier() -> void {
  // <identifier> : <type_identifier>
  if (peek(-2).get_type() != TokenType::IDENTIFIER) {
    throw_error("Expected identifier to assign type to");
    return;
  }
  if (peek(-1).get_type() != TokenType::COLON) {
    throw_error("Expected colon before type identifier");
    return;
  }
  if (peek(1).get_type() != TokenType::SEMICOLON) {
    if (peek(1).get_type() == TokenType::EQUALS) {
      add_token(peek());
      return;
    }
    throw_error("Expected assignment or termination of statement after", peek().get_lexeme());
    return;
  }
  if (peek(1).get_type() != TokenType::EQUALS) {
    if (peek(1).get_type() == TokenType::SEMICOLON) {
      add_token(peek());
      return;
    }
    throw_error("Expected assignment or termination of statement after", peek().get_lexeme());
    return;
  }
  throw_error("You really messed up");
}

inline auto Analyser::identifier() -> void {
  if (peek(-1).get_type() != TokenType::LET) {
    throw_error("Invalid use of identifier", peek().get_lexeme());
    return;
  }
  add_token(peek());
}



inline auto Analyser::validate() -> void {

  switch (const auto type = peek().get_type()) {
    case TokenType::INTEGER:
      integer();
      break;
    case TokenType::IDENTIFIER:
      identifier();
      break;
    case TokenType::TYPE_IDENTIFIER:
      type_identifier();
      break;
    default:
      if (KeywordRegistry::get_instance().is_keyword(type)) {
        keyword();
        break;
      }
      if (CharRegistry::get_instance().is_single_char(type)) {
        single_char();
        break;
      }
      throw_error("Invalid token", peek().get_lexeme());
      break;
  }
}

inline auto Analyser::analyse() -> std::vector<Token> {
  while (!is_at_end()) {
    validate();
    advance();
  }
  return tokens_out_;
}



