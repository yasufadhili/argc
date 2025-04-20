#pragma once

#include <iostream>
#include <istream>
#include <ostream>

#include "FlexLexer.h"

class Lexer : public yyFlexLexer {

public:

  Lexer(std::istream& in = std::cin, std::ostream& out = std::cout) : yyFlexLexer(in, out){}

  virtual ~Lexer(){}

};