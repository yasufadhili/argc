# Argon Compiler Documentation

Welcome to the Argon Programming Language Compiler documentation.

## Overview

The Argon compiler (argc) is a modern compiler implementation that translates Argon programming language source code into executable programs. This documentation covers the compiler's architecture, implementation details, and usage guidelines.

## Core Components

- **Lexical Analysis**: Implementation of the lexical analyzer using ANTLR4
- **Parsing**: Grammar-based parsing system
- **AST Generation**: Abstract Syntax Tree building and manipulation
- **Semantic Analysis**: Type checking and semantic validation
- **Code Generation**: Target code generation pipeline

## Getting Started

- @ref building "Building the Compiler"
- @ref usage "Using the Compiler"
- @ref development "Development Guide"
- @ref grammar "Language Grammar"

## Project Structure

The compiler is organized into several key components:

### Source Organization
- `src/` - Core compiler implementation
- `include/` - Public header files
- `grammar/` - ANTLR4 grammar definitions
- `tests/` - Test suite
- `docs/` - Documentation

### Build System
- CMake-based build system
- Cross-platform support (Windows, Linux, macOS)
- Multiple compiler support (GCC, Clang, MSVC)

## Contributing

See the @ref contributing "Contributing Guide" for information on how to contribute to the project.