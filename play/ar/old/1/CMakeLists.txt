cmake_minimum_required(VERSION 3.30)
project(Argon)

set(CMAKE_CXX_STANDARD 23)

add_executable(Argon Main.cxx
    SourceLocation.hxx
    FileHandler.hxx
    TokenType.hxx
    Token.hxx
    Registry.hxx
    Lexer.hxx
    Analyser.hxx
    AST.hxx
    Visitor.cxx
    Parser.hxx
)
