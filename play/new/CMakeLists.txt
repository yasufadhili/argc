cmake_minimum_required(VERSION 3.30)
project(New)

set(CMAKE_CXX_STANDARD 23)

add_executable(New src/Main.cxx
    src/SourceFileHandler.hxx
    src/SourceLocation.hxx
    src/TokenType.hxx
    src/Token.hxx
    src/Lexer.hxx
    src/Registry.hxx
    src/SyntaxAnalyser.hxx
    src/AST.hxx
    src/Parser.hxx
)
