%option c++
%option yyclass="Lexer"

%option warn debug batch yylineno
%option noyywrap nounput noinput nodefault

%{

#include <algorithm>
#include <iostream>
#include <fstream>
#include <stack>

#include "lexer.hh"
#include "symbols.hh"
#include "config.hh"

//config::Config config;

using namespace yy;

// Code run each time a pattern is matched.
#define YY_USER_ACTION  loc_.columns(YYLeng());

%}

%x INCLUDE

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
    loc_.step();
%}

\n              { 
                    loc_.lines(1); loc_.step(); 
                }

{WHITESPACE}+   { 
                    loc_.step(); 
                }

{COMMENT}       { /* Skip comments */ }

^[ \t]*"include"[ \t]+\"  { BEGIN(INCLUDE); }

<INCLUDE>[^\"\n]+\"[ \t]*"\n"   {
                                    std::string filename(yytext);
                                    // Remove quotes and semicolon
                                    filename = filename.substr(0, filename.find('"', 0));

                                    if (!enter_file(filename)) 
                                    {
                                        return yy::Parser::make_END(loc_);
                                    }

                                    BEGIN(INITIAL);
                                }

<INCLUDE>.|\n   { 
                    include_handler().report_error("Invalid include syntax");
                    
                    BEGIN(INITIAL);

                    return yy::Parser::make_END(loc_);
                }

"module"        {
                    if (include_handler().is_module_declared()) 
                    {
                        include_handler().report_error("Multiple module declarations in file");
                    //} else if (loc_.begin.line > 1) {
                    //    include_handler().report_error("Module declaration must be the first statement in the file");
                    }

                    include_handler().set_module_declared(true);

                    return yy::Parser::make_MODULE(loc_);
                }

"true"          { 
                    return yy::Parser::make_TRUE(loc_); 
                }

"false"         { 
                    return yy::Parser::make_FALSE(loc_); 
                }

"var"           { 
                    return yy::Parser::make_VAR(loc_); 
                }

"def"           { 
                    return yy::Parser::make_DEF(loc_); 
                }

"ret"           { 
                    return yy::Parser::make_RETURN(loc_); 
                }

"repeat"        { 
                    return yy::Parser::make_REPEAT(loc_); 
                }

"match"         { 
                    return yy::Parser::make_MATCH(loc_); 
                }

{INTEGER}       { 
                    return yy::Parser::make_INTEGER(std::atof(YYText()), loc_); 
                }

{FLOAT}         { 
                    return yy::Parser::make_FLOAT(std::atof(YYText()), loc_); 
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
    return yy::Parser::make_STRING(processed, loc_);
}

{IDENTIFIER}    { return yy::Parser::make_IDENT(std::string(YYText()), loc_); }

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
    return yy::Parser::make_CHAR(c, loc_);
}

"+"             { 
                    return yy::Parser::make_PLUS(loc_); 
                }

"-"             { 
                    return yy::Parser::make_MINUS(loc_); 
                }

"*"             { 
                    return yy::Parser::make_TIMES(loc_); 
                }

"/"             { 
                    return yy::Parser::make_DIVIDE(loc_); 
                }

"%"             { 
                    return yy::Parser::make_MODULO(loc_); 
                }

"="             { 
                    return yy::Parser::make_ASSIGN(loc_); 
                }

"=="            { 
                    return yy::Parser::make_EQ(loc_); 
                }

"!="            { 
                    return yy::Parser::make_NEQ(loc_); 
                }

">"             { 
                    return yy::Parser::make_GT(loc_); 
                }

"<"             { 
                    return yy::Parser::make_LT(loc_); 
                }

">="            { 
                    return yy::Parser::make_GEQ(loc_); 
                }

"<="            { 
                    return yy::Parser::make_LEQ(loc_); 
                }

"!"             { 
                    return yy::Parser::make_NOT(loc_); 
                }

"{"             { 
                    return yy::Parser::make_LBRACE(loc_); 
                }

"}"             { 
                    return yy::Parser::make_RBRACE(loc_); 
                }

"("             { 
                    return yy::Parser::make_LPAREN(loc_); 
                }

")"             { 
                    return yy::Parser::make_RPAREN(loc_); 
                }

";"             { 
                    return yy::Parser::make_SEMICOLON(loc_); 
                }

","             { 
                    return yy::Parser::make_COMMA(loc_); 
                }

<<EOF>>         { 
                    if (!exit_file()) 
                    { 
                        return yy::Parser::make_END(loc_);
                    }
                    
                    return yy::Parser::make_YYUNDEF(loc_);
                }

.               { 
                    include_handler().report_error("Unrecognised character: " + std::string(YYText()));

                    return yy::Parser::make_YYUNDEF(loc_);
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

bool yy::Lexer::enter_file(const std::string& filename)
{
    
    //if (config.verbose) 
    //{
    //    std::cout << "Enter file: " << filename << std::endl;
    //}

    if (!include_handler_.enter_file(filename)) {
        return false;
    }
    
    auto bs = yy_create_buffer(*include_handler_.current_file().file, YY_BUF_SIZE);

    yypush_buffer_state(bs);
    
    return true;
}

bool yy::Lexer::exit_file()
{
    if (!include_handler_.exit_file())
    {
        return false;
    }
    
    yypop_buffer_state();
    return true;
}

