
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
%token COMMA
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
%type <std::vector<std::shared_ptr<ast::func::Function>>> function_definition_list;
%type <std::shared_ptr<ast::func::Function>> function_definition;

%type <std::variant<std::shared_ptr<ast::ident::TypeIdentifier>,
                   std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>>> function_returns;
%type <std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>> function_returns_multiple_values;
%type <std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>> function_returns_multiple_values_list;
%type <std::vector<std::shared_ptr<ast::param::Parameter>>> parameter_list;
%type <std::vector<std::shared_ptr<ast::param::Parameter>>> non_empty_parameter_list;
%type <std::shared_ptr<ast::param::Parameter>> parameter;
%type <std::shared_ptr<ast::func::FunctionBody>> function_body;

%type <std::shared_ptr<ast::ident::Identifier>> identifier;
%type <std::shared_ptr<ast::ident::TypeIdentifier>> type_identifier;


%parse-param  { std::shared_ptr<ast::prog::Program>& result }



%start program




%%


program
  : function_definition_list {
    result = std::make_shared<ast::prog::Program>($1);
    $$ = result;
  }
;


function_definition_list
  : function_definition {
    $$ = std::vector<std::shared_ptr<ast::func::Function>> { $1 } ;
  }
  | function_definition_list function_definition {
    $$ = $1;
    $$.emplace_back($2);
  }
;


function_definition
  : DEF identifier LPAREN RPAREN function_returns function_body {
    if (std::holds_alternative<std::shared_ptr<ast::ident::TypeIdentifier>>($5)) {
      $$ = std::make_shared<ast::func::Function>(
        $2,
        std::vector<std::shared_ptr<ast::param::Parameter>>{},
        std::get<std::shared_ptr<ast::ident::TypeIdentifier>>($5),
        $6
      );
    } else {
      $$ = std::make_shared<ast::func::Function>(
        $2,
        std::vector<std::shared_ptr<ast::param::Parameter>>{},
        std::get<std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>>($5),
        $6
      );
    }
  }
  | DEF identifier LPAREN parameter_list RPAREN function_returns function_body {
    if (std::holds_alternative<std::shared_ptr<ast::ident::TypeIdentifier>>($6)) {
      $$ = std::make_shared<ast::func::Function>(
        $2,
        $4,
        std::get<std::shared_ptr<ast::ident::TypeIdentifier>>($6),
        $7
      );
    } else {
      $$ = std::make_shared<ast::func::Function>(
        $2,
        $4,
        std::get<std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>>($6),
        $7
      );
    }
  }
  | DEF identifier function_body {
    // No params, no return type
    $$ = std::make_shared<ast::func::Function>(
      $2,
      std::vector<std::shared_ptr<ast::param::Parameter>>{},
      std::optional<std::variant<
        std::shared_ptr<ast::ident::TypeIdentifier>,
        std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>
      >>{},
      $3
    );
  }
;


function_returns
  : type_identifier {
    $$ = $1;
  }
  | LPAREN type_identifier RPAREN {
    $$ = $2;
  }
  | function_returns_multiple_values {
    $$ = $1;
  }
  | %empty {
    // Default: no return type
    $$ = std::variant<std::shared_ptr<ast::ident::TypeIdentifier>,
          std::vector<std::shared_ptr<ast::ident::TypeIdentifier>>>{};
  }
;


function_returns_multiple_values
  : LPAREN type_identifier COMMA type_identifier RPAREN {
    $$ = std::vector<std::shared_ptr<ast::ident::TypeIdentifier>> { $2, $4 };
  }
  | LPAREN type_identifier COMMA type_identifier COMMA type_identifier RPAREN {
    $$ = std::vector<std::shared_ptr<ast::ident::TypeIdentifier>> { $2, $4, $6 };
  }
  | LPAREN function_returns_multiple_values_list RPAREN {
    $$ = $2;
  }
;


function_returns_multiple_values_list
  : type_identifier COMMA type_identifier {
    $$ = std::vector<std::shared_ptr<ast::ident::TypeIdentifier>> { $1, $3 };
  }
  | function_returns_multiple_values_list COMMA type_identifier {
    $$ = $1;
    $$.push_back($3);
  }
;


parameter_list
  : %empty {
    $$ = std::vector<std::shared_ptr<ast::param::Parameter>>{};
  }
  | non_empty_parameter_list {
    $$ = $1;
  }
;


non_empty_parameter_list
  : parameter {
    $$ = std::vector<std::shared_ptr<ast::param::Parameter>> { $1 };
  }
  | non_empty_parameter_list COMMA parameter {
    $$ = $1;
    $$.push_back($3);
  }
;


parameter
  : identifier type_identifier {
    $$ = std::make_shared<ast::param::Parameter>($1, $2);
  }
;


function_body
  : LBRACE RBRACE {
    $$ = std::make_shared<ast::func::FunctionBody>(
      std::vector<std::shared_ptr<ast::stmt::Statement>>{}
    );
  }
;


identifier
  : IDENT {
    $$ = std::make_shared<ast::ident::Identifier>($1);
  }
;


type_identifier
  : IDENT {
    $$ = std::make_shared<ast::ident::TypeIdentifier>($1);
  }
;


%%

void yy::Parser::error(const location_type& loc, const std::string& msg)
{
    std::cout << "ERROR at "<< loc << ", message: " << msg << std::endl;
}

