add_executable(argon main.cxx
    token/token.hxx
    utils/reader.hxx
    utils/writer.hxx
    lexer/lexer.cxx
    lexer/lexer.hxx
    ast/ast.hxx
    ast/visitor.cxx
    parser/parser.cxx
    parser/parser.hxx)