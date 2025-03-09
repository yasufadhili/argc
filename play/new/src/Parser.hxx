#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "AST.hxx"
#include "Token.hxx"


class ParserException : std::runtime_error{
public:

  explicit ParserException(const std::string& msg) : std::runtime_error(msg) {
    throw std::runtime_error(msg);
  }

};


class Parser {

  std::vector<Token> tokens_;
  size_t index_{0};

  auto throw_error(const std::string &msg) const -> void;
  auto throw_error(const std::string &msg, const std::string &lexeme) const -> void;

  [[nodiscard]] auto is_at_end() const -> bool;
  auto peek(const size_t &offset) -> Token;
  auto advance() -> Token;
  [[nodiscard]] auto match(const TokenType&) -> bool;
  [[nodiscard]] auto match(const std::vector<TokenType>&) -> bool;


  auto stmt() -> std::unique_ptr<ast::stmt::Stmt>;
  auto expr() -> std::unique_ptr<ast::expr::Expr>;

  auto const_expr() -> std::unique_ptr<ast::expr::Constant>;
  auto exit_stmt() -> std::unique_ptr<ast::stmt::Exit>;

public:

  explicit Parser(std::vector<Token>&& tokens) : tokens_(std::move(tokens)) {};

  auto parse() -> std::unique_ptr<ast::Node>;

};

inline auto Parser::throw_error(const std::string &msg) const -> void {
  throw ParserException(
    msg + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto Parser::throw_error(const std::string &msg, const std::string &lexeme) const -> void {
  throw ParserException(
    msg + " '" + lexeme + "' " + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto Parser::is_at_end() const -> bool {
  return tokens_.at(index_).get_type() == TokenType::END_OF_FILE;
}

inline auto Parser::peek(const size_t &offset = 0) -> Token {
  size_t idx = index_ + offset;
  if (idx >= tokens_.size()) {
    // Return EOF token if we're past the end
    return {TokenType::END_OF_FILE, "", SourceLocation(0, 0, -1)};
  }
  return tokens_.at(idx);
}

inline auto Parser::advance() -> Token {
  if (is_at_end()) {
    return tokens_.at(index_);
  }
  index_++;
  return tokens_.at(index_);
}

inline auto Parser::match(const TokenType &type) -> bool {
  return peek().get_type() == type;
}

inline auto Parser::match(const std::vector<TokenType> &types) -> bool {
  for (const auto& type : types) {
    if (peek().get_type() == type) {
      return true;
    }
  }
  return false;
}

inline auto Parser::expr() -> std::unique_ptr<ast::expr::Expr> {
  advance();
  if (peek().get_type() == TokenType::SEMICOLON) {
    advance();
  }
  return std::make_unique<ast::expr::Expr>();
}

inline auto Parser::stmt() -> std::unique_ptr<ast::stmt::Stmt> {
  advance();
  return std::make_unique<ast::stmt::Stmt>(expr());
}

inline auto Parser::parse() -> std::unique_ptr<ast::Node> {
  if (peek().get_type() != TokenType::EXIT) {
    throw_error("You messed up");
  }
  return stmt();
}


