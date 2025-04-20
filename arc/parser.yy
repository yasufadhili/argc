%require "3.2"
%language "c++"
%define parse.error verbose

%{

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>

#include "include/ast.hh"

%}


%define api.token.constructor
%define api.value.type variant


%token LPAREN RPAREN

%token PLUS MINUS
%token TIMES DIVIDE

%token <std::shared_ptr<ast::expr::Constant>> NUMBER;
%token <int8_t> I8;
%token <int16_t> I16;
%token <int32_t> I32;
%token <int64_t> I64;


%left PLUS MINUS
%left TIMES DIVIDE

%type <std::shared_ptr<ast::prog::Program>> program
%type <std::shared_ptr<ast::expr::Expression>> expression
%type <std::shared_ptr<ast::expr::Expression>> term
%type <std::shared_ptr<ast::expr::Expression>> factor

%code {
    namespace yy {
        auto yylex () -> parser::symbol_type {
            return parser::make_YYEOF ();
        }
    }
}

%%


program
   : expression-list          {

                              }
   | /** Empty**/
;

expression-list
  : expression                {}
  | expression-list           {}

expression
  : term                      {
                            
                              }
  | expression PLUS term      {
                            
                              }
  | expression MINUS term     {
                            
                              }
;

term
  : factor                    {
                                
                              }
                            
  | term TIMES factor         {
                                
                              }

  | term DIVIDE factor        {
                                
                              }
;

factor
  : NUMBER                    {
                                return std::make_unique<ast::expr::Constant>();
                              }
  | LPAREN expression RPAREN  {
                                
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

