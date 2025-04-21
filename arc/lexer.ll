%option c++
%option noyywrap

%{
#include <iostream>
#include "parser.hh"
%}

%%

"+"             { return yy::parser::token::PLUS; }

.               { std::cerr << "Unrecognised character: " << yytext << '\n'; }

%%
