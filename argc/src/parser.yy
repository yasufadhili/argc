
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
%define api.location.file "location.hh"


%param {yy::Lexer &lexer}



%code requires {

#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "include/ast.hh"
#include "include/util_logger.hh"
#include "error_handler.hh"


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


%token END 0 "end of file"
%token NEW_LINE "new line"

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
%token ASSIGN

%token BACK_TICK

%token <std::string> IDENT
%token <std::string> TYPE_IDENT

%token <int> INTEGER
%token <float> FLOAT



%token RETURN
%token VAR
%token MODULE
%token FN
%token PRINT


%parse-param  { std::shared_ptr<ast::unit::TranslationUnit>& unit }

%type <std::shared_ptr<ast::unit::TranslationUnit>> translation_unit;

%type <std::shared_ptr<ast::mod::Module>> module_definition;
%type <std::vector<std::shared_ptr<ast::mod::Module>>> module_definition_list;

%type <std::vector<std::shared_ptr<ast::stmt::Statement>>> statement_list;
%type <std::shared_ptr<ast::stmt::Statement>> statement;
%type <std::shared_ptr<ast::stmt::Statement>> execution_statement;
%type <std::shared_ptr<ast::stmt::Block>> block_statement;
%type <std::shared_ptr<ast::stmt::Statement>> control_statement;
%type <std::shared_ptr<ast::stmt::Statement>> declaration_statement;

%type <std::shared_ptr<ast::stmt::Print>> print_statement;

%type <std::shared_ptr<ast::stmt::Assignment>> assignment_statement;

%type <std::shared_ptr<ast::stmt::Return>> return_statement;
%type <std::shared_ptr<ast::stmt::VariableDeclaration>> variable_declaration;

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

%type <std::shared_ptr<ast::expr::Variable>> variable;
%type <std::shared_ptr<ast::ident::TypeIdentifier>> type_identifier;
%type <std::shared_ptr<ast::ident::Identifier>> identifier;

%type <std::shared_ptr<sym_table::Type>> type_specifier;
%type <std::optional<std::shared_ptr<ast::expr::Expression>>> optional_initialiser;


// Operator precedence - from lowest to highest
%precedence ASSIGN
%left LOGICAL_OR
%left LOGICAL_AND
%left EQ NEQ
%left GT LT GEQ LEQ
%left PLUS MINUS
%left TIMES DIVIDE MODULO
%right NOT
%precedence UNARY_MINUS


%start translation_unit




%%

translation_unit
  : module_definition_list {
    unit = std::make_shared<ast::unit::TranslationUnit>($1);
    unit->set_location(@$);
    $$ = unit;
  }
;


module_definition_list
  : module_definition {
    $$ = std::vector<std::shared_ptr<ast::mod::Module>> { $1 };
  }
  | module_definition_list module_definition {
    $$ = $1;
    $$.emplace_back($2);
  }
;


module_definition
  : MODULE identifier SEMICOLON statement_list {
    $$ = std::make_shared<ast::mod::Module>($2, $4);
    $$->set_location(@$);
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
  : declaration_statement {
    $$ = $1;
  }
  | execution_statement {
    $$ = $1;
  }
  | control_statement {
    $$ = $1;
  }
  | assignment_statement {
    $$ = $1;
  }
  | error {
    // Create a dummy statement for error recovery with location info
    error::DiagnosticHandler::instance().error("Invalid statement syntax", @$);
    $$ = std::make_shared<ast::stmt::Empty>();
    $$->set_location(@$);
  }
;


execution_statement
  : block_statement {
    $$ = $1;
  }
  | print_statement {
    $$ = $1;
  }
;


block_statement
  : LBRACE statement_list RBRACE {
    $$ = std::make_shared<ast::stmt::Block>($2);
    $$->set_location(@$);
  }
  | LBRACE RBRACE {
    $$ = std::make_shared<ast::stmt::Block>(
      std::vector<std::shared_ptr<ast::stmt::Statement>>{}
    );
    $$->set_location(@$);
  }
;


print_statement
  : PRINT expression {
    $$ = std::make_shared<ast::stmt::Print>($2);
    $$->set_location(@$);
  }
;


declaration_statement
  : variable_declaration {
    $$ = $1;
  }
;


control_statement
  : return_statement {
    $$ = $1;
  }
;


return_statement
  : RETURN arithmetic_expression {
    $$ = std::make_shared<ast::stmt::Return>($2);
    $$->set_location(@$);
  }
  | RETURN logical_expression {
    $$ = std::make_shared<ast::stmt::Return>($2);
    $$->set_location(@$);
  }
  | RETURN relational_expression {
    $$ = std::make_shared<ast::stmt::Return>($2);
    $$->set_location(@$);
  }
  | RETURN bitwise_expression {
    $$ = std::make_shared<ast::stmt::Return>($2);
    $$->set_location(@$);
  }
  | RETURN {
    $$ = std::make_shared<ast::stmt::Return>(std::nullopt);
    $$->set_location(@$);
  }
;


variable_declaration
  : VAR identifier type_specifier optional_initialiser {
    $$ = std::make_shared<ast::stmt::VariableDeclaration>($2, $3, $4);
    $$->set_location(@$);
  }
;


assignment_statement
  : identifier ASSIGN expression {
    $$ = std::make_shared<ast::stmt::Assignment>($1, $3);
    $$->set_location(@$);
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
  : TYPE_IDENT {
    $$ = std::make_shared<sym_table::Type>(sym_table::Type::TypeKind::PRIMITIVE, $1);
  }
;


expression
  : logical_expression {
    $$ = $1;
  }
  | bitwise_expression {
    $$ = $1;
  }
;


logical_expression
  : relational_expression {
    $$ = $1;
  }
  | logical_expression LOGICAL_AND relational_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::B_AND, $1, $3
    );
    $$->set_location(@$);
  }
  | logical_expression LOGICAL_OR relational_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::L_OR, $1, $3
    );
    $$->set_location(@$);
  }
;


bitwise_expression
  : arithmetic_expression BITWISE_AND arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::B_AND, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression BITWISE_OR arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::B_OR, $1, $3
    );
    $$->set_location(@$);
  }
;


relational_expression
  : arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::NONE, $1, nullptr
    );
    $$->set_location(@$);
  }
  | arithmetic_expression EQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::EQ, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression NEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::NEQ, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression GT arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::GT, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression LT arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::LT, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression LEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::LEQ, $1, $3
    );
    $$->set_location(@$);
  }
  | arithmetic_expression GEQ arithmetic_expression {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::RelationalOp::GEQ, $1, $3
    );
    $$->set_location(@$);
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
    $$->set_location(@$);
  }
  | arithmetic_expression MINUS term {
    $$ = std::make_shared<ast::expr::Binary>(
      ast::BinaryOp::SUB, $1, $3
    );
    $$->set_location(@$);
  }
;


term
  : factor {
    $$ = $1;
  }
  | term TIMES factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::MUL, $1, $3);
    $$->set_location(@$);
  }
  | term DIVIDE factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::DIV, $1, $3);
    $$->set_location(@$);
  }
  | term MODULO factor {
    $$ = std::make_shared<ast::expr::Binary>(ast::BinaryOp::MOD, $1, $3);
    $$->set_location(@$);
  }
;


unary_expression
  : TILDE factor {
    $$ = std::make_shared<ast::expr::Unary>(
      ast::UnaryOp::B_NOT, $2
    );
    $$->set_location(@$);
  }
  | MINUS factor %prec UNARY_MINUS {
    $$ = std::make_shared<ast::expr::Unary>(
      ast::UnaryOp::NEG, $2
    );
    $$->set_location(@$);
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
  | variable {
    $$ = $1;
  }
  | LPAREN arithmetic_expression RPAREN {
    $$ = $2;
    $$->set_location(@$);
  }
  | LPAREN bitwise_expression RPAREN {
    $$ = $2;
    $$->set_location(@$);
  }
;


literal
  : INTEGER {
    $$ = std::make_shared<ast::expr::Literal>($1);
    $$->set_location(@$);
  }
  | FLOAT {
    $$ = std::make_shared<ast::expr::Literal>($1);
    $$->set_location(@$);
  }
;


variable
  : identifier {
    $$ = std::make_shared<ast::expr::Variable>($1, nullptr);
    $$->set_location(@$);
  }
;


type_identifier
  : TYPE_IDENT {
    $$ = std::make_shared<ast::ident::TypeIdentifier>($1);
    $$->set_location(@$);
  }
;


identifier
  : IDENT {
    $$ = std::make_shared<ast::ident::Identifier>($1);
    $$->set_location(@$);
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
  error::DiagnosticHandler::instance().error(msg, loc);
}
