#pragma once


#include <unordered_map>

#include "TokenType.hxx"


class CharRegistry {

  std::unordered_map<char, TokenType> chars_ {
    {':', TokenType::COLON},
    {';', TokenType::SEMICOLON},
    {'(', TokenType::LPAREN},
    {')', TokenType::RPAREN},
    {'[', TokenType::LBRACK},
    {']', TokenType::RBRACK},
    {'{', TokenType::LBRACE},
    {'}', TokenType::RBRACE},
  };

public:

  static auto get_instance() -> CharRegistry & {
    static CharRegistry instance;
    return instance;
  }

  auto is_valid(const char ch) const -> bool {
    return chars_.contains(ch);
  }

  [[nodiscard]] auto get_char_type(const char ch) const -> TokenType {
    return chars_.at(ch);
  }

};


class OperatorRegistry {

  std::unordered_map<std::string, TokenType> logical_ops_ {
    {"~", TokenType::L_NOT},
    {"&", TokenType::L_AND},
    {"|", TokenType::L_OR},
    {"<<", TokenType::L_SHL},
    {">>", TokenType::L_SHR},
  };

};

class DataTypeRegistry;

class VariableRegistry;


