%require "3.2"
%language "c++"

%define api.parser.class {Parser}
%define api.value.type variant
%define api.value.automove true
%define api.token.raw
%define api.token.constructor

%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

%locations

%param {yy::Lexer &lexer}

%code requires {

#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "include/ast.hh"

namespace yy {
  class Lexer;
}
}

%code {
  #include "lexer.hh"

  yy::Parser::symbol_type yylex(yy::Lexer &lexer) {
      return lexer.lex();
  }
}

%token RPAREN LPAREN
%token PLUS MINUS TIMES DIVIDE
%token <int> INTEGER
%token <double> FLOAT
%token END 0
%token SEMICOLON

%left PLUS MINUS
%left TIMES DIVIDE

%type <std::shared_ptr<ast::prog::Program>> program;
%type <std::vector<std::shared_ptr<ast::expr::Expression>>> expression_list;
%type <std::shared_ptr<ast::expr::Expression>> expression;
%type <std::shared_ptr<ast::expr::Expression>> arithmetic_expression;
%type <std::shared_ptr<ast::expr::Constant>> constant_expression;
%type <std::shared_ptr<ast::expr::Constant>> number;
%type <std::shared_ptr<ast::expr::Expression>> factor;
%type <std::shared_ptr<ast::expr::Expression>> term;


%parse-param  { std::shared_ptr<ast::prog::Program>& result }

%start program

%%

program
  : %empty                  {
                               $$ = std::make_shared<ast::prog::Program>();
                            }

  | expression_list         {
                              result = std::make_shared<ast::prog::Program>($1);
                              $$ = result;
                            }
;

expression_list
  : expression_list expression {
      $$ = $1;
      $$.push_back($2);
    }
  | expression {
      $$ = std::vector<std::shared_ptr<ast::expr::Expression>>{$1};
    }
;

expression
  : arithmetic_expression             { $$ = $1; }
;

arithmetic_expression
  : term                              { $$ = $1; }
  | arithmetic_expression PLUS term   {
      $$ = std::make_shared<ast::expr::arith::Arithmetic>(
        ast::expr::arith::ArithmeticType::ADD, $1, $3
      );
    }
  | arithmetic_expression MINUS term  {
      $$ = std::make_shared<ast::expr::arith::Arithmetic>(
        ast::expr::arith::ArithmeticType::SUB, $1, $3
      );
    }
;

term
  : factor              { $$ = $1; }
  | term TIMES factor   {
      $$ = std::make_shared<ast::expr::arith::Arithmetic>(
        ast::expr::arith::ArithmeticType::MUL, $1, $3
      );
    }
  | term DIVIDE factor  {
      $$ = std::make_shared<ast::expr::arith::Arithmetic>(
        ast::expr::arith::ArithmeticType::DIV, $1, $3
      );
    }
;

factor
  : number                   { $$ = $1; }
  | LPAREN expression RPAREN { $$ = $2; }
;

constant_expression
  : number                   { $$ = $1; }
;

number
  : INTEGER   {
      $$ = std::make_shared<ast::expr::Constant>($1);
    }
  | FLOAT     {
      $$ = std::make_shared<ast::expr::Constant>($1);
    }
;

%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ", message: " << msg << std::endl;
}