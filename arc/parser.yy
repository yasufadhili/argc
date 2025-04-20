%require "3.2"
%language "c++"
%define parse.error verbose

%{

#include <iostream>
#include <string>

%}


%define api.token.constructor
%define api.value.type variant

%token <int> NUMBER;

%code {
    namespace yy {
        auto yylex () -> parser::symbol_type {
            return parser::make_YYEOF ();
        }
    }
}

%%

program
   : NUMBER {
                std::cout << "Number" << $1 << '\n';
            }
;

%%

namespace yy
{
  // Report an error to the user.
  auto parser::error (const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}

