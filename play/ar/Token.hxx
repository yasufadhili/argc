#pragma once

#include <string>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

#include "SourceLocation.hxx"
#include "TokenType.hxx"


class Token {
  TokenType type_;
  std::string lexeme_;
  SourceLocation location_;
  std::optional<std::variant<char, std::string> > value_;

  // Helper to convert TokenType to string for debugging
  static auto token_type_to_string(TokenType type) -> std::string {
    // Implementation will depend on TokenType enum
    // This is just a placeholder
    return std::to_string(static_cast<int>(type));
  }

public:
  Token(const TokenType type, std::string lexeme, const SourceLocation &location)
    : type_(type),
      lexeme_(std::move(lexeme)),
      location_(location) {
  }

  // Constructor for char tokens
  Token(const TokenType type, std::string lexeme, const SourceLocation &location, char char_value)
    : type_(type),
      lexeme_(std::move(lexeme)),
      location_(location),
      value_(char_value) {
  }

  // Constructor for string tokens
  Token(const TokenType type, std::string lexeme, const SourceLocation &location,
        std::string string_value)
    : type_(type),
      lexeme_(std::move(lexeme)),
      location_(location),
      value_(string_value) {
  }

  [[nodiscard]] auto get_type() const -> TokenType { return type_; }
  [[nodiscard]] auto get_lexeme() const -> const std::string & { return lexeme_; }
  [[nodiscard]] auto get_location() const -> const SourceLocation & { return location_; }

  [[nodiscard]] auto has_value() const -> bool { return value_.has_value(); }

  [[nodiscard]] auto is_char_value() const -> bool {
    return has_value() && std::holds_alternative<char>(value_.value());
  }

  [[nodiscard]] auto is_string_value() const -> bool {
    return has_value() && std::holds_alternative<std::string>(value_.value());
  }

  [[nodiscard]] auto get_char_value() const -> char {
    if (!is_char_value()) {
      throw std::runtime_error("Token does not contain a character value");
    }
    return std::get<char>(value_.value());
  }

  [[nodiscard]] auto get_string_value() const -> const std::string & {
    if (!is_string_value()) {
      throw std::runtime_error("Token does not contain a string value");
    }
    return std::get<std::string>(value_.value());
  }

  // Debug string representation
  [[nodiscard]] auto to_string() const -> std::string {
    std::string result = "Token(type=" + token_type_to_string(type_) +
                         ", lexeme='" + lexeme_ + "', " +
                         std::to_string(location_.line) + ":" + std::to_string(location_.column);

    if (has_value()) {
      if (is_char_value()) {
        const char c = get_char_value();
        result += ", char_value='";
        if (c == '\n') result += "\\n";
        else if (c == '\t') result += "\\t";
        else if (c == '\r') result += "\\r";
        else if (c == '\0') result += "\\0";
        else if (c == '\\') result += "\\\\";
        else if (c == '\'') result += "\\'";
        else result += c;
        result += "'";
      } else if (is_string_value()) {
        result += ", string_value=\"" + get_string_value() + "\"";
      }
    }

    result += ")";
    return result;
  }
};