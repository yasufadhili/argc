cmake_minimum_required(VERSION 3.15)
project(argc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Setup ANTLR4 C++ runtime library
# Check first if the submodule exists
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/antlr4/runtime/Cpp)
    message(FATAL_ERROR "ANTLR4 C++ runtime not found. Please run: git submodule update --init --recursive")
endif ()

file(GLOB ANTLR_GRAMMARS "${CMAKE_SOURCE_DIR}/grammar/*.g4")

set(ANTLR_JAR_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.13.2-complete.jar)
if (NOT EXISTS ${ANTLR_JAR_LOCATION})
    message(FATAL_ERROR "ANTLR4 JAR not found at: ${ANTLR_JAR_LOCATION}")
endif ()

# Set up ANTLR4 C++ runtime
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_INSTALL OFF)
set(ANTLR4_BUILD_CPP_TESTS OFF)
set(ANTLR4_BUILD_EXAMPLES OFF)
add_subdirectory(external/antlr4/runtime/Cpp)
set(ANTLR_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

message("Fetching fmtlib")
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)
message("Done fetching fmtlib")

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/antlr4/runtime/Cpp/runtime/src
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

function(add_antlr4_grammar)
    cmake_parse_arguments(ARG "" "OUTPUT_DIR;GRAMMAR_FILE;PACKAGE" "DEPENDS" ${ARGN})

    get_filename_component(GRAMMAR_NAME ${ARG_GRAMMAR_FILE} NAME_WE)
    set(ANTLR_OUTPUT_DIR ${ARG_OUTPUT_DIR})

    # Create generation command
    add_custom_command(
            OUTPUT
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Listener.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Listener.h
            COMMAND
            ${CMAKE_COMMAND} -E make_directory ${ANTLR_OUTPUT_DIR}
            COMMAND
            java -jar ${ANTLR_JAR_LOCATION} -Dlanguage=Cpp -listener -visitor -o ${ANTLR_OUTPUT_DIR} ${ARG_GRAMMAR_FILE}
            DEPENDS ${ARG_DEPENDS}
            COMMENT "Generating grammar: ${GRAMMAR_NAME}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    # Create a target name for the generated files
    add_custom_target(${GRAMMAR_NAME}_generate
            DEPENDS
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.h
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Listener.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Listener.h
    )
endfunction()

add_antlr4_grammar(
        GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Argon.g4
        OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Argon.g4
)


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cc")

add_executable(${PROJECT_NAME}
        ${SOURCES}
        "${ANTLR_GENERATED_DIR}/ArgonLexer.cpp"
        "${ANTLR_GENERATED_DIR}/ArgonParser.cpp"
        "${ANTLR_GENERATED_DIR}/ArgonListener.cpp"
        "${ANTLR_GENERATED_DIR}/ArgonBaseListener.cpp"
        ${PROJECT_SOURCE_DIR}/include
)

# Make sure generated parser code is created before building
add_dependencies(${PROJECT_NAME} Argon_generate)

target_link_libraries(${PROJECT_NAME} PRIVATE
        antlr4_static
        fmt::fmt
)