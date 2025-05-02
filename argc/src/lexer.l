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
#include "sym_table.hh"

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

"\n"            { 
                  loc_.lines(1); loc_.step(); 
                }

{WHITESPACE}+   { 
                    loc_.step(); 
                }

{COMMENT}       { /* Skip comments */ }

"{"             {
                  return yy::Parser::make_LBRACE(loc_);
                }

"}"             {
                  return yy::Parser::make_RBRACE(loc_);
                }

"("             {
                  return yy::Parser::make_LPAREN(loc_);
                }

")"             {
                  return yy::Parser::make_RPAREN(loc_);
                }

";"             {
                  return yy::Parser::make_SEMICOLON(loc_);
                }

"+"             {
                  return yy::Parser::make_PLUS(loc_);
                }

"-"             {
                  return yy::Parser::make_MINUS(loc_);
                }

"*"             {
                  return yy::Parser::make_TIMES(loc_);
                }

"/"             {
                  return yy::Parser::make_DIVIDE(loc_);
                }

"%"             {
                  return yy::Parser::make_MODULO(loc_);
                }

"="             {
                  return yy::Parser::make_ASSIGN(loc_);
                }

"ret"           {
                  return yy::Parser::make_RETURN(loc_);
                }

"var"           {
                  return yy::Parser::make_VAR(loc_);
                }

"module"        {
                  return yy::Parser::make_MODULE(loc_);
                }

"print"         {
                  return yy::Parser::make_PRINT(loc_);
                }

{INTEGER}       {
                    return yy::Parser::make_INTEGER(std::atof(YYText()), loc_);
                }

{FLOAT}         {
                    return yy::Parser::make_FLOAT(std::atof(YYText()), loc_);
                }

{IDENTIFIER}    {
                  auto st = sym_table::SymbolTable::get_instance();
                  if(st->lookup_type(YYText())) {
                    return yy::Parser::make_TYPE_IDENT(std::string(YYText()), loc_);
                  }
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
