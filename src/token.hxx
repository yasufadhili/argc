#pragma once

#include <string>
#include <variant>

namespace token {


  struct SourceLocation {

    std::string filename;
    size_t index{};
    size_t line{};
    size_t column{};

    explicit SourceLocation(std::string filename) {
      this->filename = std::move(filename);
    }

    SourceLocation(std::string filename, const size_t index, const size_t line, const size_t column)
      : filename(std::move(filename)), index(index), line(line), column(column) {}

    auto set_filename(std::string& name) -> void {
      filename = std::move(name);
    }

    auto set_index(const size_t index) -> void {
      this->index = index;
    }

    auto set_line(const size_t line) -> void {
      this->line = line;
    }

    auto set_column(const size_t column) -> void {
      this->column = column;
    }

  };


  enum class TokenType {
    INVALID,

    STRING, IDENTIFIER, NUMBER, KEYWORD, DELIMITER, OPERATOR, PUNCTUATOR,

    END_OF_LINE,
    END_OF_FILE,
  };

  enum class KeywordType {
    EXIT, RETURN, SET, LET
  };

  enum class IdentifierType {
    BINDING, TYPE
  };

  enum class NumberType {
    INTEGER, FLOAT
  };

  enum class OperatorType {
    // Single
    ASSIGN, PLUS, MINUS, MULTIPLY, DIVIDE,

    // Double
    EQUAL,
  };

  enum class DelimiterType {
    LPAREN, RPAREN, LBRACKET, RBRACKET, LBRACE, RBRACE
  };

  enum class PunctuatorType {
    SEMICOLON, COLON, PERIOD
  };

  using ValueVariantType = std::variant<
      IdentifierType,
      NumberType,
      KeywordType,
      OperatorType,
      PunctuatorType,
      DelimiterType,
      std::string
    >;

  struct Token {
    TokenType type;
    ValueVariantType value;
    std::string lexeme;
    SourceLocation location;
  };
}
