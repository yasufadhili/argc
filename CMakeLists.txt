cmake_minimum_required(VERSION 3.15)
project(argc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(GEN_DIR "${CMAKE_BINARY_DIR}/generated")
set(ANTLR_TOOL "${CMAKE_SOURCE_DIR}/tools/antlr-4.13.2-complete.jar")

add_subdirectory(external/antlr4/runtime/Cpp)

file(GLOB ANTLR_GRAMMARS "${CMAKE_SOURCE_DIR}/grammar/*.g4")

# Ensure generated directory exists
file(MAKE_DIRECTORY ${GEN_DIR})

# Generate sources from grammar files
set(GEN_SOURCES)
foreach(GRAMMAR ${ANTLR_GRAMMARS})
    get_filename_component(GRAMMAR_NAME ${GRAMMAR} NAME_WE)
    add_custom_command(
        OUTPUT ${GEN_DIR}/${GRAMMAR_NAME}Lexer.cpp ${GEN_DIR}/${GRAMMAR_NAME}Lexer.h
               ${GEN_DIR}/${GRAMMAR_NAME}Parser.cpp ${GEN_DIR}/${GRAMMAR_NAME}Parser.h
        COMMAND ${CMAKE_COMMAND} -E echo "Generating Antlr4 files for ${GRAMMAR_NAME}"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${GEN_DIR}
        COMMAND java -jar ${ANTLR_TOOL} -Dlanguage=Cpp -o ${GEN_DIR} ${GRAMMAR}
        DEPENDS ${GRAMMAR} ${ANTLR_TOOL}
        COMMENT "Running Antlr4 on ${GRAMMAR}"
    )
    list(APPEND GEN_SOURCES
        ${GEN_DIR}/${GRAMMAR_NAME}Lexer.cpp
        ${GEN_DIR}/${GRAMMAR_NAME}Parser.cpp
    )
endforeach()

# Gather all source files
file(GLOB SRC_FILES "${SRC_DIR}/*.cc")

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    #${GEN_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${SRC_DIR}
        ${GEN_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/antlr4/runtime/Cpp/runtime/src
        ${CMAKE_SOURCE_DIR}/external/antlr4/runtime/Cpp/runtime/src/antlr4-runtime
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE antlr4_shared
)

add_custom_target(generate_antlr
    DEPENDS ${GEN_SOURCES}
    COMMENT "Force regeneration of Antlr4 sources"
)