cmake_minimum_required(VERSION 3.30)
project(arc)

set(CMAKE_CXX_STANDARD 23)

add_executable(arc
    main.cc
    ast.hh
    ast.cc
    symbols.hh
    symbols.cc
    stmt.cc
    expr.cc
    func.cc
    prog.cc
    module.cc
)
