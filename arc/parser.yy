

/**

Match this parser to the AST with proper returns setup

**/


%require "3.2"
%language "c++"
%define parse.error verbose

%{

#include <iostream>
#include <string>
#include <cstdint>

%}


%define api.token.constructor
%define api.value.type variant


%token LPAREN RPAREN LBRACE RBRACE

%token PLUS MINUS
%token TIMES DIVIDE

%token <double> NUMBER;

%left PLUS MINUS
%left TIMES DIVIDE

%type <double> expression term factor


%code {
    namespace yy {
        auto yylex () -> parser::symbol_type {
            return parser::make_YYEOF ();
        }
    }
}

%%

program
   : expression-list
   | /** Empty**/
;

expression-list
  : expression
  | expression-list expression
;

expression
  : term                  {
                            $$ = $1;
                          }
  | expression PLUS term  {
                            $$ = $1 + $3;
                          }
  | expression MINUS term {
                            $$ = $1 - $3;
                          }
;

term
  : factor                    {
                                $$ = $1;
                              }
                            
  | term TIMES factor         {
                                $$ = $1 * $3;
                              }

  | term DIVIDE factor        {
                                if($3 == 0){
                                  error("Error: Division by zero");
                                  $$ = 0;
                                } else {
                                  $$ = $1 / $3;
                                }
                              }
;

factor
  : NUMBER                  {
                                $$ = $1;
                              }
  | LPAREN expression RPAREN  {
                                $$ = $2;
                              }
;

constant
  : NUMBER
;

%%

namespace yy
{
  // Report an error to the user.
  auto parser::error (const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}

