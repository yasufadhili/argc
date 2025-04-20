

%{

#include <iostream>

#include "parser.hh"

%}

%option noyywrap


%%

"+"             {
                  return yy::parser::token::PLUS;
                }

.               {
                  std::cerr << "Unrecognised character: " << yytext << '\n';
                }

%%

