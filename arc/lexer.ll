%option c++
%option yyclass="Lexer"
%option warn debug batch yylineno
%option noyywrap nounput noinput nodefault

%{


#include <iostream>
#include <fstream>
#include <stack>

#include "lexer.hh"

using namespace yy;

// Code run each time a pattern is matched.
#define YY_USER_ACTION  loc.columns(YYLeng());

%}

%x INCL_FILE




WHITESPACE      ([ \t\r\f]*)
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
    loc.step();
%}

\n              { loc.lines(1); loc.step(); 
                  //return yy::Parser::make_EOL(loc);
                }

{WHITESPACE}+   { loc.step(); }
{COMMENT}       { /* Skip comments */ }


include[ \t]*[\"<]            {
                            BEGIN(INCL_FILE);
                        }

<INCL_FILE>[^ \t\n\">]+          { handle_inc_file(); }

<INCL_FILE>.|\n                 { 
                                  std::cout << "{:4} bad include line" << lineno() << '\n'; 
                                  //yyterminate(); 
                                }


{INTEGER}       {
                  std::cout << "INTEGER: " << YYText() << std::endl;
                }

{FLOAT}         {
                  std::cout << "FLOAT: " << YYText() << std::endl;
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
                  std::cout << "STRING: " << processed << std::endl;
                }


{IDENTIFIER}    {
                  std::cout << "IDENT: " << YYText() << std::endl;
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
                  std::cout << "CHAR: " << c << std::endl;
                }

<<EOF>>         { 
                  if(!exit_file()) { 
                    return yy::Parser::make_YYEOF(loc);
                  } 
                }

.               { 
                  std::cerr<< ": Unrecognised character: " << YYText() << std::endl; 
                  return yy::Parser::make_YYUNDEF(loc);
                }

%%


void yy::Lexer::handle_inc_file()
{
    int c{};
    while ((c = yyinput()) && c != '\n')
        ;
    enter_file(YYText());
    BEGIN(INITIAL);
}

bool yy::Lexer::enter_file(std::string_view filename)
{
    //std::println("enter file: {}", filename);
    std::cout << "Enter file: " << filename << std::endl;

    std::ifstream file{filename.data()};
    if (file.fail())
    {
        //std::println(stderr, "open file {} failed", filename);
        std::cout << "Open file: " << filename << "failed" << std::endl;
        return false;
    }

    files.push(std::move(file));
    auto bs = yy_create_buffer(files.top(), YY_BUF_SIZE);

    yypush_buffer_state(bs);
    
    return true;
}

bool yy::Lexer::exit_file()
{
    if (files.empty())
    {
        return false;
    }
    std::cout << "Exit file: " << std::endl;
    yypop_buffer_state();
    files.pop();

    if (files.empty())
    {
        return false;
    }

    return true;
}

//yy::Lexer::~Lexer() = default;
