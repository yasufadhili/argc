
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

%token BACK_TICK


%parse-param  { std::shared_ptr<ast::unit::TranslationUnit>& unit }

%type <std::shared_ptr<ast::unit::TranslationUnit>> translation_unit;

%type <std::shared_ptr<ast::stmt::Asm>> asm_statement;


%start translation_unit




%%

translation_unit
  : asm_statement {
    unit = std::make_shared<ast::unit::TranslationUnit>($1);
    $$ = unit;
  }
;


asm_statement
  : ASM STRING_LITERAL {
    $$ = std::make_shared<ast::stmt::Asm>($2);
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ": " << msg << std::endl;
}
