#pragma once

enum class TokenType {
  INVALID,

  // Keywords
  RETURN,

  // Literals
  INT_LIT,

  //
  SEMICOLON,

  // Special
  END_OF_FILE,
};