#pragma once


#include <string>

#include "TokenType.hxx"
#include "SourceLocation.hxx"

using String = std::string;

struct Token {
  TokenType type;
  String value;
  SourceLocation location;

  Token(const TokenType type, String value, const SourceLocation &location)
    : type(type), value(std::move(value)), location(location) {
  }
};