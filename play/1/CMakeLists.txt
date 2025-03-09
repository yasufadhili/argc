cmake_minimum_required(VERSION 3.30)
project(Lang)

set(CMAKE_CXX_STANDARD 23)

add_executable(Lang src/Main.cxx
    src/TokenType.hxx
    src/SourceLocation.hxx
    src/Token.hxx
    src/Registry.hxx
    src/Lexer.hxx
    src/Visitor.hxx
    src/AST.hxx
    src/Parser.hxx
    src/FileHandler.hxx
    src/Visitor.cxx)
