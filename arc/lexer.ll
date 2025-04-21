%option c++
%option noyywrap
%option debug

%{
#include <iostream>
#include <stack>
#include <memory>
#include <string>
#include <vector>
#include "parser.hh"
#include "include/reader.hh"

// Track locations for better error reporting
#define YY_USER_ACTION \
  yylloc->first_line = yylloc->last_line; \
  yylloc->first_column = yylloc->last_column; \
  for(int i = 0; i < yyleng; ++i) { \
    if(yytext[i] == '\n') { \
      yylloc->last_line++; \
      yylloc->last_column = 0; \
    } else { \
      yylloc->last_column++; \
    } \
  }

#define YYLEXER Lexer
%}


WHITESPACE      [ \t\r\n\f]
DIGIT           [0-9]
LETTER          [a-zA-Z]
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_]*
INTEGER         {DIGIT}+
FLOAT           {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
STRING          \"([^\"\n\\]|\\[\"\\'nt])*\"
CHAR            \'([^'\n\\]|\\[\"\\'nt])\'
COMMENT         \/\/[^\n]*

%%

%{
  // Code to execute at the beginning of yylex
  col_num = yylloc->first_column;
  line_num = yylloc->first_line;
%}

{WHITESPACE}+   { /* Skip whitespace */ }
{COMMENT}       { /* Skip comments */ }

include[ ]+\"([^\"]+)\" {
  std::string fname(yytext + 8, yyleng - 9); // Extract filename
  this->include_file(fname);
  // No token returned for include
}

"int"           { return yy::parser::token::TYPE_INT; }
"float"         { return yy::parser::token::TYPE_FLOAT; }
"char"          { return yy::parser::token::TYPE_CHAR; }
"string"        { return yy::parser::token::TYPE_STRING; }
"void"          { return yy::parser::token::TYPE_VOID; }

"if"            { return yy::parser::token::IF; }
"else"          { return yy::parser::token::ELSE; }
"while"         { return yy::parser::token::WHILE; }
"for"           { return yy::parser::token::FOR; }
"return"        { return yy::parser::token::RETURN; }
"break"         { return yy::parser::token::BREAK; }
"continue"      { return yy::parser::token::CONTINUE; }

"+"             { return yy::parser::token::PLUS; }
"-"             { return yy::parser::token::MINUS; }
"*"             { return yy::parser::token::MUL; }
"/"             { return yy::parser::token::DIV; }
"%"             { return yy::parser::token::MOD; }

"=="            { return yy::parser::token::EQ; }
"!="            { return yy::parser::token::NE; }
"<"             { return yy::parser::token::LT; }
"<="            { return yy::parser::token::LE; }
">"             { return yy::parser::token::GT; }
">="            { return yy::parser::token::GE; }

"&&"            { return yy::parser::token::AND; }
"||"            { return yy::parser::token::OR; }
"!"             { return yy::parser::token::NOT; }

"="             { return yy::parser::token::ASSIGN; }

"{"             { return yy::parser::token::LBRACE; }
"}"             { return yy::parser::token::RBRACE; }
"("             { return yy::parser::token::LPAREN; }
")"             { return yy::parser::token::RPAREN; }
"["             { return yy::parser::token::LBRACKET; }
"]"             { return yy::parser::token::RBRACKET; }
";"             { return yy::parser::token::SEMICOLON; }
","             { return yy::parser::token::COMMA; }

{IDENTIFIER}    { 
  yylval->emplace<std::string>(yytext, yyleng);
  return yy::parser::token::IDENTIFIER; 
}

{INTEGER}       { 
  yylval->emplace<int>(std::stoi(std::string(yytext, yyleng)));
  return yy::parser::token::INT_LITERAL; 
}

{FLOAT}         { 
  yylval->emplace<double>(std::stod(std::string(yytext, yyleng)));
  return yy::parser::token::FLOAT_LITERAL; 
}

{STRING}        { 
  // Remove quotes and handle escape sequences
  std::string str(yytext + 1, yyleng - 2);
  std::string processed;
  for (size_t i = 0; i < str.length(); ++i) {
    if (str[i] == '\\' && i + 1 < str.length()) {
      ++i;
      switch (str[i]) {
        case 'n': processed += '\n'; break;
        case 't': processed += '\t'; break;
        case '\\': processed += '\\'; break;
        case '\"': processed += '\"'; break;
        case '\'': processed += '\''; break;
        default: processed += str[i]; break;
      }
    } else {
      processed += str[i];
    }
  }
  yylval->emplace<std::string>(std::move(processed));
  return yy::parser::token::STRING_LITERAL; 
}

{CHAR}          { 
  char c = yytext[1];
  if (c == '\\' && yyleng > 3) {
    switch (yytext[2]) {
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
      case '\\': c = '\\'; break;
      case '\"': c = '\"'; break;
      case '\'': c = '\''; break;
      default: c = yytext[2]; break;
    }
  }
  yylval->emplace<char>(c);
  return yy::parser::token::CHAR_LITERAL; 
}

<<EOF>> {
  if (this->pop_include()) {
      // Continue lexing previous file
      return yylex(yylval, yylloc);
  }
  return 0; // End of all input
}

. { 
    std::cerr << get_position_info() << ": Unrecognised character: " << yytext << std::endl; 
    return yy::parser::token::ERROR;
}

%%

// Implementation of Lexer constructor if needed outside of the definitions above
//Lexer::Lexer(std::istream* in) : yyFlexLexer(in), line_num(1), col_num(1) {}

#include "parser.hh"
yy::parser::symbol_type yylex(Lexer& lexer) {
    return lexer.yylex();
}