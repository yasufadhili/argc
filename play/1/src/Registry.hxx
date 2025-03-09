#pragma once

#include <string>
#include <unordered_map>

#include "TokenType.hxx"

using String = std::string;

class CharRegistry {
  std::unordered_map<char, TokenType> chars;

public:
  CharRegistry() {
    add(';', TokenType::SEMICOLON);
  }

  static auto get_instance() -> CharRegistry & {
    static CharRegistry instance;
    return instance;
  }

  auto add(const char ch, const TokenType type) -> void {
    chars[ch] = type;
  }

  auto get_char_type(const char ch) const -> TokenType {
    return chars.at(ch);
  }

  auto is_char(const char ch) const -> bool {
    return chars.contains(ch);
  }
};


class KeywordsRegistry {
  std::unordered_map<String, TokenType> keywords;

public:
  KeywordsRegistry() {
    add("return", TokenType::RETURN);
  }

  static auto get_instance() -> KeywordsRegistry & {
    static KeywordsRegistry instance;
    return instance;
  }

  auto get_token_type(const String &name) const -> TokenType {
    auto it = keywords.find(name);
    if (it != keywords.end()) {
      return it->second;
    }
    return TokenType::INVALID;
  }

  auto add(const String &name, const TokenType type) -> void {
    keywords[name] = type;
  }

  auto is_keyword(const String &name) const -> bool {
    return keywords.contains(name);
  }
};
