%{
#include <iostream>
#include <string>

enum TokenType {
  TOKEN_INTEGER,
  TOKEN_FLOAT,
  TOKEN_STRING,
  TOKEN_IDENTIFIER,
  TOKEN_OPERATOR,
  TOKEN_COMMENT,
  TOKEN_EOF,
  TOKEN_KEYWORD,
  TOKEN_UNKNOWN
};

void print_token(TokenType type, const char* lexeme, int line) {
  const char* token_names[] = {
    "INTEGER", "FLOAT", "STRING", "IDENTIFIER", 
    "OPERATOR", "COMMENT", "EOF", "KEYWORD", "UNKNOWN"
  };
  
  std::cout << "Line " << line << ": " << token_names[type] << " [" << lexeme << "]" << std::endl;
}

int line_num = 1;
%}

%option noyywrap
%option outfile="lexer.cc"


DIGIT       [0-9]
LETTER      [a-zA-Z]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+([eE][-+]?{DIGIT}+)?
STRING      \"([^\"\n]|\\.)*\"
WHITESPACE  [ \t\r]+
OPERATOR    [\+\-\*\/\%\=\<\>\!\&\|\^\~\(\)\[\]\{\}\,\;\:\.]
SINGLECOMM  \/\/.*
MULTICOMM   \/\*([^*]|\*[^\/])*\*\/
KEYWORD     if|else|while|for|return|int|float|void|char|struct|class|const|static|break|continue|switch|case|default|do|enum|extern|goto|long|register|short|signed|sizeof|typedef|union|unsigned|volatile

%%
{KEYWORD}    { print_token(TOKEN_KEYWORD, yytext, line_num); }
{INTEGER}    { print_token(TOKEN_INTEGER, yytext, line_num); }
{FLOAT}      { print_token(TOKEN_FLOAT, yytext, line_num); }
{STRING}     { print_token(TOKEN_STRING, yytext, line_num); }
{IDENTIFIER} { print_token(TOKEN_IDENTIFIER, yytext, line_num); }
{OPERATOR}   { print_token(TOKEN_OPERATOR, yytext, line_num); }
{SINGLECOMM} { print_token(TOKEN_COMMENT, yytext, line_num); }
{MULTICOMM}  { 
    for(int i = 0; yytext[i] != '\0'; i++) {
        if(yytext[i] == '\n') line_num++;
    }
    print_token(TOKEN_COMMENT, yytext, line_num); 
}
\n          { line_num++; }
{WHITESPACE} { /* ignore whitespace */ }
<<EOF>>     { print_token(TOKEN_EOF, "EOF", line_num); return 0; }
.           { print_token(TOKEN_UNKNOWN, yytext, line_num); }
%%