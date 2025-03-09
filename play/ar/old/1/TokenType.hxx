#pragma once



enum class TokenType {

  INVALID,

  // Chars
  COLON, SEMICOLON, EQUALS,

  //Delimiters
  LBRACE, RBRACE, LPAREN, RPAREN, LBRACK, RBRACK,

  //Keywords
  EXIT, LET, SET,

  IDENTIFIER, TYPE_IDENTIFIER,

  // Literals
  STRING, INTEGER, FLOAT,

  END_OF_LINE,
  END_OF_FILE,

};

