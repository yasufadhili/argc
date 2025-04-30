
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

%token SEMICOLON

%token BACK_TICK

%token <std::string> IDENT

%token <int> INTEGER


%parse-param  { std::shared_ptr<ast::unit::TranslationUnit>& unit }

%type <std::shared_ptr<ast::unit::TranslationUnit>> translation_unit;

%type <std::shared_ptr<ast::expr::Expression>> expression;

%type <std::shared_ptr<ast::expr::Expression>> term;
%type <std::shared_ptr<ast::expr::Expression>> factor;

%type <std::shared_ptr<ast::ident::Identifier>> identifier;
%type <std::shared_ptr<ast::expr::Literal>> literal;


%left PLUS MINUS
%left TIMES DIVIDE MODULO


%start translation_unit




%%

translation_unit
  : identifier {
    unit = std::make_shared<ast::unit::TranslationUnit>();
    $$ = unit;
  }
;


identifier
  : IDENT {
    $$ = std::make_shared<ast::ident::Identifier>($1);
  }
;

expression
  : term {
    $$ = $1;
  }
  | expression PLUS term {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::Add, $1, $3);
  }
  | expression MINUS term {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::Sub, $1, $3);
  }
;

term
  : term TIMES factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::Mul, $1, $3);
  }
  | term TIMES factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::Div, $1, $3);
  }
  | factor {
    $$ = $1;
  }
;


factor
  : literal {
    $$ = $1;
  }
  | LPAREN expression RPAREN {
    $$ = $2;
  }
;


literal
  : INTEGER {
    $$ = std::make_shared<ast::expr::Literal>($1);
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ": " << msg << std::endl;
}
