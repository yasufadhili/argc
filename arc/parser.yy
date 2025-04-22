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
%token LBRACE RBRACE
%token PLUS MINUS
%token TIMES DIVIDE
%token <int> INTEGER
%token <double> FLOAT
%token END 0
%token SEMICOLON
%token VAR
%token DEF

%token TRUE FALSE
%token ASSIGN EQ NEQ GT LT GEQ LEQ
%token NOT

%token <std::string> IDENT
%token <std::string> TYPE_IDENT
%token <std::string> STRING
%token <char> CHAR



%precedence ASSIGN
%left EQ NEQ
%left GT LT GEQ LEQ
%left PLUS MINUS
%left TIMES DIVIDE
%right NOT



%type <std::shared_ptr<ast::prog::Program>> program;
%type <std::shared_ptr<ast::ident::Identifier>> identifier;


%parse-param  { std::shared_ptr<ast::prog::Program>& result }



%start program




%%

program
  : %empty {
     $$ = std::make_shared<ast::prog::Program>();
  }
  | identifier {

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
    std::cout << "ERROR at "<< loc << ", message: " << msg << std::endl;
}