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
%token LBRACE RBRACE
%token PLUS MINUS
%token TIMES DIVIDE
%token <int> INTEGER
%token <double> FLOAT
%token END 0
%token SEMICOLON
%token VAR

%token TRUE FALSE
%token ASSIGN EQ NE GT LT GEQ LEQ
%token NOT

%token <std::string> IDENT
%token <std::string> STRING
%token <char> CHAR

%precedence ASSIGN
%left EQ NEQ
%left GT LT GEQ LEQ
%left PLUS MINUS
%left TIMES DIVIDE
%right NOT

%type <std::shared_ptr<ast::prog::Program>> program;
%type <std::vector<std::shared_ptr<ast::stmt::Statement>>> statement_list;
%type <std::shared_ptr<ast::stmt::Statement>> statement;
%type <std::shared_ptr<ast::stmt::Block>> block;
%type <std::shared_ptr<ast::stmt::VariableDeclaration>> var_declaration;
%type <std::optional<std::shared_ptr<ast::expr::Expression>>> optional_initialiser;
%type <std::shared_ptr<sym::Type>> type_specifier;
%type <std::shared_ptr<ast::expr::Expression>> expression;
%type <std::shared_ptr<ast::expr::Expression>> arithmetic_expression;
%type <std::shared_ptr<ast::expr::Constant>> constant;
%type <std::shared_ptr<ast::expr::Constant>> number;
%type <std::shared_ptr<ast::expr::Expression>> factor;
%type <std::shared_ptr<ast::expr::Expression>> term;
%type <std::shared_ptr<ast::expr::Variable>> variable;
%type <std::shared_ptr<ast::expr::boolean::Boolean>> boolean_expression;
%type <std::shared_ptr<ast::expr::Unary>> unary_expression;
%type <std::shared_ptr<ast::expr::rel::Relational>> relational_expression;

%parse-param  { std::shared_ptr<ast::prog::Program>& result }

%start program

%%

program
  : %empty                  {
                               $$ = std::make_shared<ast::prog::Program>();
                            }
  | statement_list         {
                              result = std::make_shared<ast::prog::Program>($1);
                              $$ = result;
                            }
;

statement_list
  : statement {
    $$ = std::vector<std::shared_ptr<ast::stmt::Statement>>{$1};
  }
  | statement_list statement {
    $$ = $1;
    $$.push_back($2);
  }
;

statement
  : var_declaration {
    $$ = $1;
  }
  | expression {
    $$ = std::make_shared<ast::stmt::ExpressionStatement>($1);
  }
  | block {
    $$ = $1;
  }
;

block
  : LBRACE statement_list RBRACE {
      $$ = std::make_shared<ast::stmt::Block>($2);
  }
  | LBRACE RBRACE {
    $$ = std::make_shared<ast::stmt::Block>(
      std::vector<std::shared_ptr<ast::stmt::Statement>>{}
    );
  }
;


var_declaration
  : VAR IDENT type_specifier optional_initialiser {
    $$ = std::make_shared<ast::stmt::VariableDeclaration>($2, $3, $4);
  }
;


optional_initialiser
  : %empty {
    $$ = std::nullopt;
  }
  | ASSIGN expression {
    $$ = $2;
  }
;


type_specifier
  : IDENT {
      $$ = std::make_shared<sym::Type>(sym::Type::TypeKind::PRIMITIVE, $1);
  }
;


expression
  : arithmetic_expression { $$ = $1; }
  | relational_expression { $$ = $1; }
  | variable { $$ = $1; }
  | constant { $$ = $1; }
;


boolean_expression
  : TRUE                            { 
                                      $$ = std::make_shared<ast::expr::boolean::Boolean>
                                      (
                                        ast::expr::boolean::BooleanType::TRUE
                                      ); 
                                    }
  | FALSE                           { 
                                      $$ = std::make_shared<ast::expr::boolean::Boolean>
                                      (
                                        ast::expr::boolean::BooleanType::FALSE
                                      ); 
                                    }
;


unary_expression
  : NOT expression                  { 
      $$ = std::make_shared<ast::expr::Unary>(
        ast::expr::Unary::UnaryOp::NOT, $2
      ); 
    }
  | MINUS expression %prec NOT      { 
      $$ = std::make_shared<ast::expr::Unary>(
        ast::expr::Unary::UnaryOp::NEG, $2
      ); 
    }
;


relational_expression
  : expression EQ expression        { 
      $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::EQ, $1, $3
      ); 
    }
  | expression NEQ expression        { 
      $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::NE, $1, $3
      ); 
    }
  | expression GT expression        { 
      $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::GT, $1, $3
      ); 
    }
  | expression LT expression        { 
      $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::LT, $1, $3
      ); 
    }
  | expression LEQ expression {
    $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::LE, $1, $3
    );
  }
  | expression GEQ expression {
    $$ = std::make_shared<ast::expr::rel::Relational>(
        ast::expr::rel::RelationalType::GE, $1, $3
    );
  }
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


number
  : INTEGER {
    $$ = std::make_shared<ast::expr::Constant>(
      $1,
      sym::Type::TypeKind::PRIMITIVE
    );
  }

  | FLOAT {
    $$ = std::make_shared<ast::expr::Constant>(
      $1,
      sym::Type::TypeKind::PRIMITIVE
    );
  }
;

variable
  : IDENT {
    $$ = std::make_shared<ast::expr::Variable>($1);
  }
;

constant
  : INTEGER {
    $$ = std::make_shared<ast::expr::Constant>(
      $1,
      sym::Type::TypeKind::PRIMITIVE
    );
  }

  | FLOAT {
    $$ = std::make_shared<ast::expr::Constant>(
      $1,
      sym::Type::TypeKind::PRIMITIVE
    );
  }

  | STRING {
    $$ = std::make_shared<ast::expr::Constant>(
      $1,
      sym::Type::TypeKind::STRING
    );
  }

  | TRUE {
    $$ = std::make_shared<ast::expr::Constant>(
      true,
      sym::Type::TypeKind::BOOL
    );
  }
  | FALSE {
    $$ = std::make_shared<ast::expr::Constant>(
      false,
      sym::Type::TypeKind::BOOL
    );
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ", message: " << msg << std::endl;
}