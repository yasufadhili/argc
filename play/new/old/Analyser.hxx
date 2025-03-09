#pragma once

#include <utility>
#include <unordered_map>
#include <vector>
#include <stack>

#include "Registry.hxx"
#include "TokenType.hxx"
#include "Token.hxx"


class SyntaxError final : std::runtime_error {
public:
  explicit SyntaxError(const std::string &msg) : std::runtime_error(msg) {
    throw std::runtime_error("SyntaxError: " + msg);
  }
};


class Analyser {
  std::vector<Token> tokens_;
  std::vector<Token> tokens_out_;
  size_t index_{0};

  std::unordered_map<TokenType, TokenType> delimiter_pairs_ {
      {TokenType::LPAREN, TokenType::RPAREN},
      {TokenType::LBRACE, TokenType::RBRACE},
      {TokenType::LBRACKET, TokenType::RBRACKET}
  };

  std::stack<TokenType> delimiter_stack_;

  auto throw_error(const std::string &msg) const -> void;
  auto throw_error(const std::string &msg, const std::string& lexeme) const -> void;

  [[nodiscard]] auto is_at_end() const -> bool;
  auto peek(const size_t &offset) -> Token;
  auto advance() -> Token;
  [[nodiscard]] auto match(const TokenType&) -> bool;
  [[nodiscard]] auto match(const std::vector<TokenType>&) -> bool;
  auto add_token(const Token&) -> void;
  auto add_token(const TokenType&) -> void;

  auto let() -> void;
  auto exit() -> void;
  auto set() -> void;
  auto identifier() -> void;
  auto type() -> void;
  auto assign() -> void;
  auto integer() -> void;
  auto identifier_usage() -> void;

  auto delimiter() -> void;
  auto is_delimiter(const TokenType& type) const -> bool;
  static auto is_open_delimiter(const TokenType& type) -> bool;
  auto is_close_delimiter(const TokenType& type) -> bool;

  auto validate() -> void;

public:
  explicit Analyser(std::vector<Token> tokens);

  auto analyse() -> std::vector<Token>;
  auto validate_tokens() -> bool;
};

inline Analyser::Analyser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

inline auto Analyser::throw_error(const std::string &msg) const -> void {
  throw SyntaxError(msg + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
}

inline auto Analyser::throw_error(const std::string &msg, const std::string &lexeme) const -> void {
  throw SyntaxError(msg + " '" + lexeme + "' " + " at " + std::to_string(tokens_.at(index_).get_location().line) + ":" + std::to_string(tokens_.at(index_).get_location().column));
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

inline auto Analyser::validate() -> void {
  switch (peek().get_type()) {
    case TokenType::LET:
      let();
    break;
    case TokenType::EXIT:
      exit();
    break;
    case TokenType::SET:
      set();
    break;
    case TokenType::INTEGER:
      integer();
    break;
    case TokenType::IDENTIFIER:
      identifier_usage();
    break;
    case TokenType::LPAREN:
    case TokenType::LBRACE:
    case TokenType::LBRACKET:
    case TokenType::RPAREN:
    case TokenType::RBRACE:
    case TokenType::RBRACKET:
      delimiter();
    break;
    default:
      throw_error("Unexpected token", peek().get_lexeme());
  }
}

inline auto Analyser::identifier_usage() -> void {
  add_token(peek());
  advance(); // Consume identifier

  // Handle assignment to existing variable
  if (match(TokenType::EQUALS)) {
    add_token(peek()); // Add the equals sign
    advance(); // Consume equals

    if (match(TokenType::INTEGER)) {
      add_token(peek()); // Add the integer value
      advance(); // Consume integer

      if (!match(TokenType::SEMICOLON)) {
        throw_error("Expected semicolon after assignment");
      }
      add_token(peek()); // Add semicolon
      advance(); // Consume semicolon
    } else if (match(TokenType::SET)) {
      set();
    } else {
      throw_error("Expected value after assignment operator");
    }
  } else {
    throw_error("Unexpected identifier usage", peek(-1).get_lexeme());
  }
}

inline auto Analyser::let() -> void {
  add_token(peek());
  advance(); // Consume the let keyword
  if (!match(TokenType::IDENTIFIER)) {
    throw_error("Expected identifier after 'let' keyword");
  }
  identifier();
}

inline auto Analyser::identifier() -> void {
  add_token(peek());
  advance(); // Consume the identifier
  if (!match(TokenType::COLON)) {
    throw_error("Expected ':' after identifier");
  }
  type();
}

inline auto Analyser::type() -> void {
  add_token(peek()); // add the :
  advance(); // Consume the :
  if (!match(TokenType::IDENTIFIER)) {
    throw_error("Expected type identifier after ':'");
  }
  if (!DataTypeRegistry::get_instance().is_data_type_registered(peek().get_lexeme())) {
    throw_error("Unknown data type ", peek().get_lexeme());
  }
  add_token(peek());
  advance(); // Consume the type identifier
  if (match(TokenType::SEMICOLON)) {
    add_token(peek());
    advance(); // Consume ;
  } else if (match(TokenType::EQUALS)) {
    assign();
  } else {
    throw_error("Expected semicolon or assignment after type identifier");
  }
}

inline auto Analyser::assign() -> void {
  add_token(peek());
  advance(); // Consume the =
  if (match(TokenType::SET)) {
    set();
  } else {
    throw_error("Expected function binding after assignment ");
  }
}

inline auto Analyser::set() -> void {
  add_token(peek());
  advance(); // consume 'set'
  if (!match(TokenType::LPAREN)) {
    throw_error("Expected '(' after 'set' keyword");
  }
  add_token(peek());
  advance(); // consume '(
  if (!match(TokenType::RPAREN)) {
    throw_error("Expected ')' after '('");
  }
  add_token(peek());
  advance(); // consume ')'
  if (!match(TokenType::LBRACE)) {
    throw_error("Expected '{' after ')' to start function body");
  }
  add_token(peek());
  delimiter_stack_.push(TokenType::LBRACE);
  advance(); // consume '{'
}

inline auto Analyser::is_delimiter(const TokenType& type) const -> bool {
  return delimiter_pairs_.contains(type);
}

inline auto Analyser::delimiter() -> void {
  if (is_open_delimiter(peek().get_type())) {
    // Track the delimiter for later validation
    delimiter_stack_.push(peek().get_type());
    add_token(peek());
    advance(); // Consume the delimiter

    // Handle code blocks with nested statements
    if (peek().get_type() == TokenType::LBRACE) {
      // Parse statements inside braces until we reach closing brace
      while (!is_at_end() && peek().get_type() != TokenType::RBRACE) {
        validate();
        if (is_at_end()) break;
        advance();
      }

      if (is_at_end() && peek().get_type() != TokenType::RBRACE) {
        throw_error("Unterminated code block, expected '}'");
      }
    }
  } else if (is_close_delimiter(peek().get_type())) {
    if (delimiter_stack_.empty()) {
      throw_error("Unexpected closing delimiter", peek().get_lexeme());
    }

    auto top = delimiter_stack_.top();
    TokenType expected_open {};

    for (const auto& [open, close] : delimiter_pairs_) {
      if (close == peek().get_type()) {
        expected_open = open;
        break;
      }
    }

    if (top != expected_open) {
      throw_error("Mismatched closing delimiter", peek().get_lexeme());
    }

    delimiter_stack_.pop();
    add_token(peek());
    advance(); // Consume the delimiter
  }
}

inline auto Analyser::is_open_delimiter(const TokenType& type) -> bool {
  return type == TokenType::LPAREN || type == TokenType::LBRACE || type == TokenType::LBRACKET;
}

inline auto Analyser::is_close_delimiter(const TokenType& type) -> bool {
  for (auto &[open, close] : delimiter_pairs_) {
    if (close == type) {
      return true;
    }
  }
  return false;
}

inline auto Analyser::exit() -> void {
  add_token(peek());
  advance(); // Consume the exit keyword
  if (!match(TokenType::INTEGER)) {
    throw_error("Can only exit with integer literal");
  }
  add_token(peek());
  advance(); // Consume int literal
  if (!match(TokenType::SEMICOLON)) {
    throw_error("Expected semicolon after exit keyword");
  }
  add_token(peek()); // Add the semicolon token
  advance(); // Consume semicolon
}

inline auto Analyser::integer() -> void {
  add_token(peek());
  advance(); // Consume the integer

  if (!match(TokenType::SEMICOLON)) {
    throw_error("Expected semicolon after integer literal");
  }
  add_token(peek()); // Add the semicolon
  advance(); // Consume semicolon
}


inline auto Analyser::analyse() -> std::vector<Token> {
  while (!is_at_end()) {
    validate();
    advance();
  }

  if (!delimiter_stack_.empty()) {
    throw_error("Unclosed delimiter");
  }

  add_token(TokenType::END_OF_FILE);
  return tokens_out_;
}

inline auto Analyser::validate_tokens() -> bool {
  size_t original_index = index_;
  std::vector<Token> original_tokens_out = tokens_out_;

  try {
    analyse();
    // Restore original state
    index_ = original_index;
    tokens_out_ = original_tokens_out;
    return true;
  } catch (const SyntaxError&) {
    // Restore original state
    index_ = original_index;
    tokens_out_ = original_tokens_out;
    return false;
  }
}

