
#include <iostream>
#include "FlexLexer.h"

#include "include/reader.hh"

extern int yylex();

auto main(int argc, char **argv) -> int {
    std::unique_ptr<utils::Reader> reader;
    std::istream* input = nullptr;
    try {
        if (argc > 1) {
            reader = std::make_unique<utils::Reader>(argv[1]);
        } else {
            reader = std::make_unique<utils::Reader>("");
        }
        input = &reader->stream();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Create the Flex C++ lexer and set its input stream
    yyFlexLexer lexer(input);
    int token;
    while ((token = lexer.yylex()) != 0) {
        std::cout << "Token: " << token << std::endl;
    }
    return 0;
}