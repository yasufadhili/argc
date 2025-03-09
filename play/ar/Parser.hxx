#pragma once

#include <stdexcept>
#include <vector>
#include <memory>

#include "Token.hxx"
#include "AST.hxx"


class ParserException {
public:
  explicit ParserException(const std::string &msg) {
    fprintf(stderr, " %s", msg.c_str());
    exit(1);
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

  [[nodiscard]] auto match(const TokenType &) -> bool;

  [[nodiscard]] auto match(const std::vector<TokenType> &) -> bool;

  [[nodiscard]] auto match(const std::string &) -> bool;

  auto expr() -> std::shared_ptr<ast::expr::Expr>;
  auto stmt() -> std::shared_ptr<ast::stmt::Stmt>;

  auto literal() -> std::shared_ptr<ast::expr::Literal>;

  auto exit() -> std::shared_ptr<ast::stmt::Exit>;

  auto program() -> std::shared_ptr<Prog>

public:
  explicit Parser(std::vector<Token> &&tokens) : tokens_(std::move(tokens)) {
  };

  auto parse() -> std::shared_ptr<ast::Node>;
};

inline auto Parser::throw_error(const std::string &msg) const -> void {
  fprintf(stderr, " %s at line %d:%d",
    msg.c_str(),
    static_cast<int>(tokens_.at(index_).get_location().line),
    static_cast<int>(tokens_.at(index_).get_location().column)
    );
  ::exit(EXIT_FAILURE);
}

inline auto Parser::throw_error(const std::string &msg, const std::string &lexeme) const -> void {
  fprintf(stderr, " %s '%s' at line %d:%d",
   msg.c_str(),
   lexeme.c_str(),
   static_cast<int>(tokens_.at(index_).get_location().line),
   static_cast<int>(tokens_.at(index_).get_location().column)
   );
  ::exit(EXIT_FAILURE);
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

inline auto Parser::exit() -> std::shared_ptr<ast::stmt::Exit> {
  return std::make_shared<ast::stmt::Exit>(peek(1).get_lexeme());
}


inline auto Parser::parse() -> std::shared_ptr<ast::Node> {

  TokenType type = peek().get_type();
  if (type == TokenType::EXIT) {
    return exit();
  }

  throw_error("Unsupported usage");
  return nullptr;

}


