#pragma once

#include "location.hh"
#include <stack>
#include <fstream>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"

namespace yy {


class Lexer : public yyFlexLexer {
  std::stack<std::ifstream> files;
  yy::location loc;
  public:

      bool enter_file(std::string_view filename);
      bool exit_file();

      using yyFlexLexer::yyFlexLexer;

      void initialize_location(yy::location::filename_type &f, yy::location::counter_type l = 1, yy::location::counter_type c = 1)
      {
          loc.initialize(&f, l, c);
      }

  private:
      void handle_inc_file();

  public:
    yy::Parser::symbol_type lex();
};

}

#undef YY_DECL
#define YY_DECL yy::Parser::symbol_type yy::Lexer::lex()
