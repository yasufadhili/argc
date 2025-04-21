%require "3.2"
%language "c++"

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include "lexer.hh"
}

%code {
    // Forward declaration for C++ Bison
    static yy::parser::symbol_type yylex(Lexer& lexer);
}

%locations


%token TYPE_INT TYPE_FLOAT TYPE_CHAR TYPE_STRING TYPE_VOID
%token IF ELSE WHILE FOR RETURN BREAK CONTINUE
%token PLUS MINUS MUL DIV MOD
%token EQ NE LT LE GT GE
%token AND OR NOT
%token ASSIGN
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET
%token SEMICOLON COMMA
%token ERROR
%token <std::string> IDENTIFIER
%token <int> INT_LITERAL
%token <double> FLOAT_LITERAL
%token <std::string> STRING_LITERAL
%token <char> CHAR_LITERAL


%%

program: statement_list
       ;

statement_list: statement
              | statement_list statement
              ;

statement: /* Future grammar rules here */
         ;

%%

void yy::parser::error(const location_type& loc, const std::string& msg) {
  std::cerr << loc.begin.filename << ":" << loc.begin.line << ":" 
            << loc.begin.column << ": " << msg << std::endl;
}