cmake_minimum_required(VERSION 3.15)

# Prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()


project(argc 
    VERSION 0.1.0
    DESCRIPTION "The Argon Programming Language Compiler"
    HOMEPAGE_URL "https://github.com/yasufadhili/argc"
    LANGUAGES CXX
)

# Add cmake modules path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Include platform detection early
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DetectPlatform.cmake)

# Compiler requirements
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0)
        message(FATAL_ERROR "GCC version must be at least 11.0!")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.0)
        message(FATAL_ERROR "Clang version must be at least 13.0!")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.29)
        message(FATAL_ERROR "MSVC version must be at least 19.29 (VS 2019 16.9)!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang, GCC, and MSVC.")
endif()

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Global compilation settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform-specific compiler flags and settings
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/W4 /WX /MP)
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
else()
    add_compile_options(
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror
        $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
        $<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>
    )
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build options
option(ARGC_BUILD_TESTS "Build tests" ON)
option(ARGC_BUILD_DOCS "Build documentation" OFF)
option(ARGC_ENABLE_LTO "Enable Link Time Optimization" OFF)

# Enable LTO if requested
if(ARGC_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
    if(IPO_SUPPORTED)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "LTO is not supported: ${IPO_ERROR}")
    endif()
endif()

# ANTLR4 setup
set(ANTLR_RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/antlr4/runtime/Cpp)
set(ANTLR_JAR_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr-4.13.2-complete.jar)
set(ANTLR_GENERATED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")

if (NOT EXISTS ${ANTLR_RUNTIME_DIR})
    message(FATAL_ERROR "ANTLR4 C++ runtime not found. Please run: git submodule update --init --recursive")
endif ()

if (NOT EXISTS ${ANTLR_JAR_LOCATION})
    message(FATAL_ERROR "ANTLR4 JAR not found at: ${ANTLR_JAR_LOCATION}")
endif ()


# Ensure Java is available
find_program(JAVA_EXECUTABLE java)
if (NOT JAVA_EXECUTABLE)
    message(FATAL_ERROR "Java not found. Please install Java or add it to PATH.")
endif()


# ANTLR4 runtime configuration
set(ANTLR4_WITH_STATIC_CRT ON)
set(ANTLR4_INSTALL OFF)
set(ANTLR4_BUILD_CPP_TESTS OFF)
set(ANTLR4_BUILD_EXAMPLES OFF)
add_subdirectory(${ANTLR_RUNTIME_DIR})


# Dependencies
message(STATUS "Fetching dependencies...")
include(FetchContent)

# fmt library
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

# CLI11 for command line parsing
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(CLI11)

# spdlog for logging
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# Project structure
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${ANTLR_RUNTIME_DIR}/runtime/src
    ${ANTLR_GENERATED_DIR}
)


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


file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cc")
file(GLOB_RECURSE ANTLR_GENERATED_SRC "${ANTLR_GENERATED_DIR}/*.cpp")
file(GLOB_RECURSE ANTLR_GENERATED_HDR "${ANTLR_GENERATED_DIR}/*.h")


# Grammar as a separate library
add_library(argon_grammar STATIC ${ANTLR_GENERATED_SRC})
target_include_directories(argon_grammar PUBLIC ${ANTLR_GENERATED_DIR})
add_dependencies(argon_grammar Argon_generate)


# Main executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE
    argon_grammar
    antlr4_static
    fmt::fmt
    CLI11::CLI11
    spdlog::spdlog
)


# Installation
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


# Testing
if(ARGC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()


# Documentation
if(ARGC_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    else()
        message(WARNING "Doxygen not found. Documentation will not be built.")
    endif()
endif()


# Export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


