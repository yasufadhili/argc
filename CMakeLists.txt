cmake_minimum_required(VERSION 3.30)
project(Argon)

set(CMAKE_CXX_STANDARD 23)

add_executable(argon 
    main.cxx
    src/ast.hxx
    src/token.hxx
    src/parser.hxx
    src/visitor.cxx
    src/file_utils.hxx
)
