
%require "3.2"
%language "c++"

%define api.parser.class {Parser}
%define api.value.type variant
//%define api.value.automove true
%define api.token.raw
%define api.token.constructor

%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

%locations
%define api.location.file "location.hh"


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


%token END 0

%token ASM
%token <std::string> STRING_LITERAL

%token LBRACE
%token RBRACE
%token RPAREN
%token LPAREN

%token PLUS
%token MINUS
%token TIMES
%token DIVIDE
%token MODULO

%token UNARY_MINUS
%token TILDE

%token SEMICOLON

%token BACK_TICK

%token <std::string> IDENT

%token <int> INTEGER


%parse-param  { std::shared_ptr<ast::unit::TranslationUnit>& unit }

%type <std::shared_ptr<ast::unit::TranslationUnit>> translation_unit;

%type <std::shared_ptr<ast::expr::Expression>> expression;
%type <std::shared_ptr<ast::expr::Unary>> unary_expression;
%type <std::shared_ptr<ast::expr::Expression>> logical_expression;
%type <std::shared_ptr<ast::expr::Expression>> arithmetic_expression;
%type <std::shared_ptr<ast::expr::Expression>> relational_expression;

%type <std::shared_ptr<ast::expr::Expression>> term;
%type <std::shared_ptr<ast::expr::Expression>> factor;
%type <std::shared_ptr<ast::expr::Expression>> primary;
%type <std::shared_ptr<ast::expr::Literal>> literal;
%type <std::shared_ptr<ast::expr::Literal>> boolean_literal;

%type <std::shared_ptr<ast::ident::Identifier>> identifier;


%left PLUS MINUS
%left TIMES DIVIDE MODULO


%start translation_unit




%%

translation_unit
  : expression {
    unit = std::make_shared<ast::unit::TranslationUnit>($1);
    $$ = unit;
  }
;


expression
  : term {
    $$ = $1;
  }
  | expression PLUS term {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::ADD, $1, $3);
  }
  | expression MINUS term {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::SUB, $1, $3);
  }
;


arithmetic_expression
  : term {
    $$ = $1;
  }
  | arithmetic_expression PLUS term {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::ADD, $1, $3
    );
  }
  | arithmetic_expression MINUS term {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::SUB, $1, $3
    );
  }
;


term
  : factor {
    $$ = $1;
  }
  | term TIMES factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::MUL, $1, $3);
  }
  | term TIMES factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::DIV, $1, $3);
  }
  | term MODULO factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::MOD, $1, $3);
  }
;


unary_expression
  : TILDE factor {
    $$ = std::make_shared<ast::expr::Unary>(
      ast::UnaryOp::B_NOT, $2
    );
  }
  | MINUS factor %prec UNARY_MINUS {
    $$ = std::make_shared<ast::expr::Unary>(
      ast::UnaryOp::NEG, $2
    );
  }
;


factor
  : primary {
    $$ = $1;
  }
  | unary_expression {
    $$ = $1;
  }
;


primary
  : literal {
    $$ = $1;
  }
  | LPAREN arithmetic_expression RPAREN {
    $$ = $2;
  }
;


literal
  : INTEGER {
    $$ = std::make_shared<ast::expr::Literal>($1);
  }
;


identifier
  : IDENT {
    $$ = std::make_shared<ast::ident::Identifier>($1);
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ": " << msg << std::endl;
}
