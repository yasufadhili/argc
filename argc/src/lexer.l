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
STRING_LIT      \`([]|\\[\"\\'nt])*\`
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

"asm"           {
                  return yy::Parser::make_ASM(loc_);
                }

{STRING_LIT}    {
                  std::string str(yytext + 1, yyleng - 2);
                  std::string processed;
                  for (size_t i = 0; i < str.length(); ++i) {
                    if (str[i] == '\\' && i + 1 < str.length()) {
                      ++i;
                      switch (str[i]) {
                        case 'n': processed += '\n'; break;
                        case 't': processed += '\t'; break;
                        case '\\': processed += '\\'; break;
                        case '\"': processed += '\"'; break;
                        case '\'': processed += '\''; break;
                        default: processed += str[i]; break;
                      }
                    } else {
                      processed += str[i];
                    }
                  }
                  return yy::Parser::make_STRING_LITERAL(processed, loc_);
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
