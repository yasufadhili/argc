
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



%token RPAREN ")"
%token LPAREN "("
%token LBRACE "{"
%token RBRACE "}"
%token PLUS "+"
%token MINUS "-"
%token TIMES "*"
%token DIVIDE "/"
%token MODULO "%"
%token <int> INTEGER  "integer"
%token <double> FLOAT "float"
%token END 0
%token SEMICOLON  ";"
%token COMMA ","
%token VAR  "variable declaration"
%token DEF  "function definition"
%token RETURN "return"

%token REPEAT "repeat"

%token TRUE "true"
%token FALSE "false"
%token ASSIGN "assign"
%token EQ "=="
%token NEQ "!="
%token GT ">"
%token LT "<"
%token GEQ ">="
%token LEQ "<="
%token NOT "!"

%token <std::string> IDENT "identifier"
%token <std::string> TYPE_IDENT "type identifier"
%token <std::string> STRING "string"
%token <char> CHAR "char"



// Operator precedence - from lowest to highest
%precedence ASSIGN
%left OR
%left AND
%left EQ NEQ
%left GT LT GEQ LEQ
%left PLUS MINUS
%left TIMES DIVIDE MODULO
%right NOT
%precedence UNARY_MINUS



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
%type <std::shared_ptr<ast::stmt::Block>> function_body;
%type <std::shared_ptr<ast::stmt::Return>> return_statement;

%type <std::vector<std::shared_ptr<ast::stmt::Statement>>> statement_list;
%type <std::shared_ptr<ast::stmt::Statement>> statement;
%type <std::shared_ptr<ast::stmt::Block>> block_statement;
%type <std::shared_ptr<ast::stmt::VariableDeclaration>> variable_declaration;

%type <std::shared_ptr<ast::stmt::Repeat>> repeat_statement;

%type <std::shared_ptr<ast::ident::Identifier>> identifier;
%type <std::shared_ptr<ast::ident::TypeIdentifier>> type_identifier;

%type <std::optional<std::shared_ptr<ast::expr::Expression>>> optional_initialiser;
%type <std::shared_ptr<ast::stmt::Assignment>> assignment;
%type <std::shared_ptr<sym::Type>> type_specifier;

%type <std::shared_ptr<ast::expr::Expression>> expression;

%type <std::shared_ptr<ast::expr::boolean::Boolean>> boolean_expression;
%type <std::shared_ptr<ast::expr::Unary>> unary_expression;

%type <std::shared_ptr<ast::expr::Expression>> arithmetic_expression;
%type <std::shared_ptr<ast::expr::rel::Relational>> relational_expression;

//%type <std::shared_ptr<sym::Type>> type_specifier;
%type <std::shared_ptr<ast::expr::Variable>> variable;

%type <std::shared_ptr<ast::expr::Expression>> factor;
%type <std::shared_ptr<ast::expr::Expression>> term;

%type <std::shared_ptr<ast::expr::Constant>> number;
%type <std::shared_ptr<ast::expr::Constant>> constant;
%type <std::shared_ptr<ast::expr::Constant>> boolean_constant;


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
  : block_statement {
    $$ = $1;
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
  : variable_declaration {
    $$ = $1;
  }
  | expression {
    $$ = std::make_shared<ast::stmt::ExpressionStatement>($1);
  }
  | assignment {
      $$ = $1;
  }
  | block_statement {
    $$ = $1;
  }
  | return_statement {
    $$ = $1;
  }
  | repeat_statement {
    $$ = $1;
  }
;


block_statement
  : LBRACE statement_list RBRACE {
      $$ = std::make_shared<ast::stmt::Block>($2);
  }
  | LBRACE RBRACE {
    $$ = std::make_shared<ast::stmt::Block>(
      std::vector<std::shared_ptr<ast::stmt::Statement>>{}
    );
  }
;


variable_declaration
  : VAR IDENT type_specifier optional_initialiser {
    $$ = std::make_shared<ast::stmt::VariableDeclaration>($2, $3, $4);
  }
;


return_statement
  : RETURN expression {
    $$ = std::make_shared<ast::stmt::Return>($2);
  }
  | RETURN {
    $$ = std::make_shared<ast::stmt::Return>(std::nullopt);
  }
;


repeat_statement
  : REPEAT block_statement {
    $$ = std::make_shared<ast::stmt::Repeat>(std::nullopt);
  }
  | REPEAT arithmetic_expression block_statement {
    $$ = std::make_shared<ast::stmt::Repeat>($2);
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
      // Think validation should be done in semantic analysis
      $$ = std::make_shared<sym::Type>(sym::TypeKind::PRIMITIVE, $1);
  }
;


assignment
  : IDENT ASSIGN expression {
    $$ = std::make_shared<ast::stmt::Assignment>($1, $3);
  }
;


expression
  : arithmetic_expression {
    $$ = $1;
  }
  | relational_expression {
    $$ = $1;
  }
  | unary_expression {
    $$ = $1;
  }
  | boolean_expression {
    $$ = $1;
  }
  | variable {
    $$ = $1;
  }
  | constant {
    $$ = $1;
  }
;


boolean_expression
  : TRUE {
    $$ = std::make_shared<ast::expr::boolean::Boolean>(ast::expr::boolean::BooleanType::TRUE);
  }
  | FALSE {
    $$ = std::make_shared<ast::expr::boolean::Boolean>( ast::expr::boolean::BooleanType::FALSE );
  }
;


unary_expression
  : NOT expression {
    $$ = std::make_shared<ast::expr::Unary>(
      ast::expr::Unary::UnaryOp::NOT, $2
    );
  }
  | MINUS expression %prec NOT {
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
      ast::expr::rel::RelationalType::NEQ, $1, $3
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
      ast::expr::rel::RelationalType::LEQ, $1, $3
    );
  }
  | expression GEQ expression {
    $$ = std::make_shared<ast::expr::rel::Relational>(
      ast::expr::rel::RelationalType::GEQ, $1, $3
    );
  }
;


arithmetic_expression
  : term {
    $$ = $1;
  }
  | arithmetic_expression PLUS term {
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
  : factor {
    $$ = $1;
  }
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
  | term MODULO factor  {
    $$ = std::make_shared<ast::expr::arith::Arithmetic>(
      ast::expr::arith::ArithmeticType::MOD, $1, $3
    );
  }
;


factor
  : number {
    $$ = $1;
  }
  | LPAREN expression RPAREN {
    $$ = $2;
  }
;


number
  : INTEGER {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
  | FLOAT {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
;


variable
  : IDENT {
    $$ = std::make_shared<ast::expr::Variable>($1);
  }
;


constant
  : INTEGER {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
  | FLOAT {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
  | STRING {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
  | CHAR {
    $$ = std::make_shared<ast::expr::Constant>($1, sym::TypeKind::PRIMITIVE);
  }
  | boolean_constant {
    $$ = $1;
  }
;


boolean_constant
  : TRUE {
    $$ = std::make_shared<ast::expr::Constant>(true, sym::TypeKind::BOOL);
  }
  | FALSE {
    $$ = std::make_shared<ast::expr::Constant>(false, sym::TypeKind::BOOL);
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
    std::cout << "ERROR at "<< loc << ": " << msg << std::endl;
}

