%option c++
%option yyclass="Lexer"

%option warn debug batch yylineno
%option noyywrap nounput noinput nodefault

%{

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stack>

#include "lexer.hh"

using namespace yy;

// Code run each time a pattern is matched.
#define YY_USER_ACTION  loc_.columns(YYLeng());

%}

%x INCLUDE

WHITESPACE      ([ \t\r\f]*)
DIGIT           [0-9]
LETTER          [a-zA-Z]
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_]*
INTEGER         {DIGIT}+
FLOAT           {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
STRING          \"([^\"\n\\]|\\[\"\\'nt])*\"
STRING_LIT      \`([^\"\n\\]|\\[\"\\'nt])*\`
CHAR            \'([^'\n\\]|\\[\"\\'nt])\'
COMMENT         \/\/[^\n]*

%%

%{
    loc_.step();
%}

\n              { 
                    loc_.lines(1); loc_.step(); 
                }

{WHITESPACE}+   { 
                    loc_.step(); 
                }

{COMMENT}       { /* Skip comments */ }

{INTEGER}       {
                    return yy::Parser::make_INTEGER(std::atof(YYText()), loc_);
                }

{FLOAT}         {
                    return yy::Parser::make_FLOAT(std::atof(YYText()), loc_);
                }

{IDENTIFIER}    {
                  return yy::Parser::make_IDENT(std::string(YYText()), loc_);
                }

<<EOF>>         { 
                    if (!exit_file()) 
                    { 
                        return yy::Parser::make_END(loc_);
                    }
                    
                    return yy::Parser::make_YYUNDEF(loc_);
                }

.               { 
                    include_handler().report_error("Unrecognised character: " + std::string(YYText()));

                    return yy::Parser::make_YYUNDEF(loc_);
                }

%%


void yy::Lexer::handle_inc_file()
{
  int c{};
  while ((c = yyinput()) && c != '\n')
      ;
  enter_file(YYText());
  BEGIN(INITIAL);
}

bool yy::Lexer::enter_file(const std::string& filename)
{

  if (!include_handler_.enter_file(filename)) {
      return false;
  }
  
  auto bs = yy_create_buffer(*include_handler_.current_file().file, YY_BUF_SIZE);

  yypush_buffer_state(bs);
  
  return true;
}

bool yy::Lexer::exit_file()
{
  if (!include_handler_.exit_file())
  {
      return false;
  }
  
  yypop_buffer_state();
  return true;
}
