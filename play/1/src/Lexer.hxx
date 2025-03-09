#pragma once

#include <stdexcept>
#include <string_view>
#include <vector>

#include "Token.hxx"
#include "Registry.hxx"


using StringView = std::string_view;
using RuntimeError = std::runtime_error;


class Lexer {
  StringView src{};
  size_t index{0};
  size_t line{1};
  size_t column{1};

  using TokenVector = std::vector<Token>;
  TokenVector tokens{};

  [[nodiscard]] auto is_end() const -> bool {
    return index >= src.length();
  }

  [[nodiscard]] auto peek(const size_t offset = 0) const -> char {
    if (index + offset >= src.length()) {
      return '\0';
    }
    return src[index + offset];
  }

  auto advance() -> char {
    if (is_end()) {
      return '\0';
    }
    const char c = peek();
    index++;
    column++;

    if (c == '\n') {
      line++;
      column = 1;
    }

    return c;
  }

  auto add_token(TokenType type, const String &value) -> void {
    tokens.emplace_back(type, value, SourceLocation(line, column - value.length(), index - value.length()));
  }

  auto identifier() -> void {
    String value;
    while (!is_end() && (std::isalnum(peek()) || peek() == '_')) {
      value += advance();
    }

    if (KeywordsRegistry::get_instance().is_keyword(value)) {
      add_token(KeywordsRegistry::get_instance().get_token_type(value), value);
    } //else {
    //add_token(TokenType::IDENTIFIER, value);
    //}
  }

  auto integer() -> void {
    String value;
    while (!is_end() && std::isdigit(peek())) {
      value += advance();
    }
    add_token(TokenType::INT_LIT, value);
  }

  auto skip_whitespace() -> void {
    while (!is_end() && std::isspace(peek())) {
      advance();
    }
  }

  auto scan_token() -> void {
    skip_whitespace();

    if (is_end()) {
      add_token(TokenType::END_OF_FILE, "");
      return;
    }

    const char c = advance();

    if (std::isalpha(c) || c == '_') {
      // We've already consumed one character, so move back
      index--;
      column--;
      identifier();
    } else if (std::isdigit(c)) {
      index--;
      column--;
      integer();
    } else if (CharRegistry::get_instance().is_char(c)) {
      add_token(CharRegistry::get_instance().get_char_type(c), String(1, c));
    } else {
      throw RuntimeError("Unexpected character '" + String(1, c) +
                         "' at line " + std::to_string(line) +
                         ", column " + std::to_string(column - 1));
    }
  }

public:
  explicit Lexer(const StringView &src) : src(src) {
  }

  auto lex() -> TokenVector {
    while (!is_end()) {
      scan_token();
    }

    if (tokens.empty() || tokens.back().type != TokenType::END_OF_FILE) {
      add_token(TokenType::END_OF_FILE, "");
    }

    return tokens;
  }
};
