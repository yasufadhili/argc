
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

%token LT
%token GT
%token EQ
%token LEQ
%token GEQ
%token NEQ

%token UNARY_MINUS
%token TILDE

%token LOGICAL_AND
%token LOGICAL_OR

%token BITWISE_AND
%token BITWISE_OR

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
%type <std::shared_ptr<ast::expr::Expression>> bitwise_expression;
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
    unit = std::make_shared<ast::unit::TranslationUnit>();
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


logical_expression
  : relational_expression {
    $$ = $1;
  }
  | logical_expression LOGICAL_AND relational_expression {
    $$ = std::make_shared<ast::Binary>(
      ast::BinaryOp::B_AND, $1, $3
    );
  }
  | logical_expression LOGICAL_OR relational_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::L_OR, $1, $3
    );
  }
;


bitwise_expression
  : arithmetic_expression BITWISE_AND arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::B_AND, $1, $3
    );
  }
  | arithmetic_expression BITWISE_OR arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::B_OR, $1, $3
    );
  }
;


relational_expression
  : arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::NONE, $1, nullptr
    );
  }
  | arithmetic_expression EQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::EQ, $1, $3
    );
  }
  | arithmetic_expression NEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::NEQ, $1, $3
    );
  }
  | arithmetic_expression GT arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::GT, $1, $3
    );
  }
  | arithmetic_expression LT arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::LT, $1, $3
    );
  }
  | arithmetic_expression LEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::LEQ, $1, $3
    );
  }
  | arithmetic_expression GEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::GEQ, $1, $3
    );
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
