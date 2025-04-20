%require "3.2"
%language "c++"
%define parse.error verbose

%{

#include <iostream>
#include <string>
#include <cstdint>

#include "include/ast.hh"

%}


%define api.token.constructor
%define api.value.type variant


%token LPAREN RPAREN

%token PLUS MINUS
%token TIMES DIVIDE

%token <double> NUMBER;
%token <int8_t> I8;
%token <int16_t> I16;
%token <int32_t> I32;
%token <int64_t> I64;


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
   : expression
   | /** Empty**/
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
  : NUMBER                    {
                                $$ = $1;
                              }
  | LPAREN expression RPAREN  {
                                $$ = $2;
                              }
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

