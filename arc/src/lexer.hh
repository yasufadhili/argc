#pragma once

#include "location.hh"  
#include "include/include_handler.hh"
#include <stack>
#include <fstream>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hh"

namespace yy {

class Lexer : public yyFlexLexer {
private:
    IncludeHandler include_handler_;
    yy::location loc_;

public:
    using yyFlexLexer::yyFlexLexer;

    void initialize_location(yy::location::filename_type &f, yy::location::counter_type l = 1, yy::location::counter_type c = 1) {
        loc_.initialize(&f, l, c);
    }

    bool enter_file(const std::string& filename);

    bool exit_file();

    void handle_inc_file();

    yy::Parser::symbol_type lex();

    const IncludeHandler& include_handler() const { return include_handler_; }
    IncludeHandler& include_handler() { return include_handler_; }
};

}

#undef YY_DECL
#define YY_DECL yy::Parser::symbol_type yy::Lexer::lex()
