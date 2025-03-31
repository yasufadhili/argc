
set(ARGON_SOURCES
    main.cxx
    lexer/lexer.cxx
    ast/visitor.cxx
    parser/parser.cxx
)

set(ARGON_HEADERS
    token/token.hxx
    utils/reader.hxx
    utils/writer.hxx
    lexer/lexer.hxx
    ast/ast.hxx
    parser/parser.hxx
    registry/registry.hxx
)

add_executable(argon ${ARGON_SOURCES} ${ARGON_HEADERS})

target_include_directories(argon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add compile definitions (later)
# target_compile_definitions(argon PRIVATE SOME_DEFINITION=1)


target_compile_options(argon PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)


set_target_properties(argon PROPERTIES
    OUTPUT_NAME "argon"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Install target (will be used by the top-level CMakeLists.txt)
export(TARGETS argon FILE argon-config.cmake)