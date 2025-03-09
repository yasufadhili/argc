#pragma once



enum class TokenType {

  INVALID,

  // Delimiters
  LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,

  // Single Char tokens
  COLON, SEMICOLON, EQUALS,

  // Double Char Tokens
  EQUAL_EQUAL,

  // Literals
  INTEGER, IDENTIFIER, FLOAT,

  TYPE_IDENTIFIER,

  //Keywords
  EXIT, LET, SET,

  END_OF_FILE

};
