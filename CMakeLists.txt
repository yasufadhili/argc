cmake_minimum_required(VERSION 3.15)
project(argc LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Force static runtime library on Windows with MSVC to match ANTLR4
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # Alternative approach: modify compiler flags directly
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()


# Additional module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# ANTLR4 setup
set(ANTLR_RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/antlr4/runtime/Cpp)
set(ANTLR_JAR_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.13.2-complete.jar)
set(ANTLR_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")


# Dummy file just to pass initial CMake configuration
file(MAKE_DIRECTORY ${ANTLR_GENERATED_DIR})
file(TOUCH ${ANTLR_GENERATED_DIR}/Dummy.cpp)


if (NOT EXISTS ${ANTLR_RUNTIME_DIR})
    message(FATAL_ERROR "ANTLR4 C++ runtime not found. Please run: git submodule update --init --recursive")
endif ()

if (NOT EXISTS ${ANTLR_JAR_LOCATION})
    message(FATAL_ERROR "ANTLR4 JAR not found at: ${ANTLR_JAR_LOCATION}")
endif ()


# Respect JAVA_HOME if available
if(DEFINED ENV{JAVA_HOME})
    set(JAVA_HOME_HINT "$ENV{JAVA_HOME}/bin")
else()
    set(JAVA_HOME_HINT "")
endif()

find_program(JAVA_EXECUTABLE
    NAMES java
    HINTS ${JAVA_HOME_HINT}
    PATHS /usr/bin /usr/local/bin
    DOC "Java executable used to run ANTLR"
)

if (NOT JAVA_EXECUTABLE)
    message(FATAL_ERROR "Java not found. Please install Java and ensure it's on your PATH or set JAVA_HOME.")
endif()

# Show Java version
execute_process(
    COMMAND ${JAVA_EXECUTABLE} -version
    OUTPUT_VARIABLE _java_out
    ERROR_VARIABLE _java_err
    RESULT_VARIABLE _java_result
)

if(NOT _java_result EQUAL 0)
    message(FATAL_ERROR "Java executable found at ${JAVA_EXECUTABLE} but failed to run: ${_java_err}")
else()
    message(STATUS "Found Java: ${JAVA_EXECUTABLE}")
    message(STATUS "Java version info: ${_java_err}")
endif()


# ANTLR4 runtime configuration
set(ANTLR4_WITH_STATIC_CRT ON)
set(ANTLR4_INSTALL OFF)
set(ANTLR4_BUILD_CPP_TESTS OFF)
set(ANTLR4_BUILD_EXAMPLES OFF)
add_subdirectory(${ANTLR_RUNTIME_DIR})


message("Fetching fmtlib")
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)
message("Done fetching fmtlib")


# Grammar generation function
function(add_antlr4_grammar)
    cmake_parse_arguments(ARG "" "OUTPUT_DIR;GRAMMAR_FILE" "DEPENDS" ${ARGN})

    get_filename_component(GRAMMAR_NAME ${ARG_GRAMMAR_FILE} NAME_WE)
    set(ANTLR_OUTPUT_DIR ${ARG_OUTPUT_DIR})

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
            COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR_OUTPUT_DIR}
            COMMAND ${JAVA_EXECUTABLE} -jar ${ANTLR_JAR_LOCATION} -Dlanguage=Cpp -listener -visitor -o ${ANTLR_OUTPUT_DIR} ${ARG_GRAMMAR_FILE}
            DEPENDS ${ARG_DEPENDS}
            COMMENT "Generating grammar: ${GRAMMAR_NAME}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(${GRAMMAR_NAME}_generate
            DEPENDS
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Lexer.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Parser.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}BaseListener.cpp
            ${ANTLR_OUTPUT_DIR}/${GRAMMAR_NAME}Listener.cpp
    )

    set_source_files_properties(${ARG_GRAMMAR_FILE} PROPERTIES GENERATED TRUE)
endfunction()


# Generate grammar from Argon.g4
add_antlr4_grammar(
        GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Argon.g4
        OUTPUT_DIR ${ANTLR_GENERATED_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Argon.g4
)


file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cc")
file(GLOB_RECURSE ANTLR_GENERATED_SRC "${ANTLR_GENERATED_DIR}/*.cpp")
file(GLOB_RECURSE ANTLR_GENERATED_HDR "${ANTLR_GENERATED_DIR}/*.h")


# Grammar as a separate library
add_library(argon_grammar STATIC ${ANTLR_GENERATED_SRC}  )
target_include_directories(argon_grammar PUBLIC ${ANTLR_GENERATED_DIR})
add_dependencies(argon_grammar Argon_generate)


# Main executable
add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_SOURCE_DIR}/include
)


add_dependencies(${PROJECT_NAME} Argon_generate)


target_include_directories(argon_grammar PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ANTLR_RUNTIME_DIR}/runtime/src
    ${ANTLR_GENERATED_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ANTLR_GENERATED_DIR}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
        argon_grammar
        antlr4_static
        fmt::fmt
)


message("Fetching GoogleTest")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)
message("Done fetching GoogleTest")


enable_testing()


add_executable(
    test_symbol_table
    tests/SymbolTableTests.cc
)

target_include_directories(
  test_symbol_table
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(test_symbol_table PRIVATE
        GTest::gtest
        GTest::gtest_main
)



add_test(NAME SymbolTableTests COMMAND test_symbol_table)


set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

