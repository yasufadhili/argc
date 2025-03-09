#pragma once

#include <string>
#include <unordered_map>

#include "TokenType.hxx"


class CharRegistry {
  std::unordered_map<TokenType, char> single_chars_{
    {TokenType::COLON, ':'},
    {TokenType::SEMICOLON, ';'},
    {TokenType::EQUALS, '='},
  };

  std::unordered_map<TokenType, std::string> double_chars_{
    {TokenType::EQUAL_EQUAL, "=="},
  };

  std::unordered_map<TokenType, std::string> delimiters_{
    {TokenType::LPAREN, "("},
    {TokenType::RPAREN, ")"},
    {TokenType::LBRACE, "{"},
    {TokenType::RBRACE, "}"},
    {TokenType::LBRACKET, "["},
    {TokenType::RBRACKET, "]"},
  };

public:
  static auto get_instance() -> CharRegistry & {
    static CharRegistry instance;
    return instance;
  }

  auto is_single_char(const TokenType type) const -> bool {
    return single_chars_.contains(type);
  }

  auto is_double_char(const TokenType type) const -> bool {
    return double_chars_.contains(type);
  }

  auto is_delimiter(const TokenType type) const -> bool {
    return delimiters_.contains(type);
  }

};


class KeywordRegistry {

  std::unordered_map<TokenType, std::string> keywords_{
    {TokenType::LET, "let"},
    {TokenType::SET, "set"},
    {TokenType::EXIT, "exit"}
  };

public:

  static auto get_instance() -> KeywordRegistry & {
    static KeywordRegistry instance;
    return instance;
  }

  auto is_keyword(const TokenType type) const -> bool {
    return keywords_.contains(type);
  }

};


class DataTypeRegistry {
  std::unordered_map<std::string, size_t> data_types;

public:
  DataTypeRegistry() {
    add_data_type("i8", 1);
    add_data_type("i16", 2);
    add_data_type("i32", 4);
    add_data_type("i64", 8);
  };

  static auto get_instance() -> DataTypeRegistry & {
    static DataTypeRegistry instance;
    return instance;
  }

  auto add_data_type(const std::string &name, const size_t size_bytes) -> void {
    data_types[name] = size_bytes;
  }

  auto get_data_type_size(const std::string &name) const -> size_t {
    return data_types.at(name);
  }

  auto is_data_type_registered(const std::string &name) const -> bool {
    return data_types.contains(name);
  }

};
