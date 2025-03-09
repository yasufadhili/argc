#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Token.hxx"
#include "AST.hxx"

using RuntimeError = std::runtime_error;


class Parser {
  std::vector<Token> tokens;
  size_t index{0};

  auto peek(const size_t offset = 0) -> Token & {
    return tokens[index + offset];
  }

  auto advance() -> void {
    index++;
  }

  auto match(const TokenType type) -> bool {
    if (peek().type == type) {
      advance();
      return true;
    }
    return false;
  };

  auto match(const TokenType type, const String &message) -> void {
    if (peek().type == type) {
      advance();
      return;
    }
    throw RuntimeError(message);
  }

  auto consume(TokenType type) -> void {
    if (peek().type == type) {
      advance();
      return;
    }
    throw RuntimeError("Expected '" + std::string(1, static_cast<char>(type)) + "' but got '" + peek().value + "'");
  }


  //auto consume(TokenType type, const String &message) -> void;
  //auto consume_keyword(const String &name) -> void;
  //auto consume_keyword(const String &name, const String &message) -> void;

  auto prog() -> std::shared_ptr<ast::prog::Program>;
  auto unit() -> std::shared_ptr<ast::unit::Unit>;
  auto func() -> std::shared_ptr<ast::func::Func>;
  auto stmt() -> std::shared_ptr<ast::stmt::Stmt>;
  auto expr() -> std::shared_ptr<ast::expr::Expr>;

  auto int_lit() -> std::shared_ptr<ast::expr::IntLit>;
  auto return_stmt() -> std::shared_ptr<ast::stmt::Return>;

public:
  explicit Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

  auto parse() -> std::shared_ptr<ast::prog::Program> {
    return prog();
  }

};

inline auto Parser::int_lit() -> std::shared_ptr<ast::expr::IntLit> {
  const std::string val = peek().value;
  advance();
  return std::make_shared<ast::expr::IntLit>(std::stoi(val));
}

inline auto Parser::expr() -> std::shared_ptr<ast::expr::Expr> {
  return int_lit();
}


inline auto Parser::return_stmt() -> std::shared_ptr<ast::stmt::Return> {
  if (peek().type == TokenType::INT_LIT) {
    return std::make_shared<ast::stmt::Return>(int_lit());
  }
  return std::make_shared<ast::stmt::Return>(nullptr);
}

inline auto Parser::stmt() -> std::shared_ptr<ast::stmt::Stmt> {
  if (peek().type == TokenType::RETURN) {
    advance();
    return return_stmt();
  }
  throw RuntimeError("Unexpected statement at " + std::to_string(peek().location.line) + "." + std::to_string(peek().location.column));
}

inline auto Parser::func() -> std::shared_ptr<ast::func::Func> {
  std::vector<std::shared_ptr<ast::stmt::Stmt>> stmts;
  for (auto stmt : stmt()) {
    stmts.push_back(stmt);
  }
  return std::make_shared<ast::func::Func>();
}





