# Argon Project Structure Documentation

This document provides a detailed overview of the Argon compiler's architecture and code organization.

## Overview

The Argon compiler is structured as a traditional compiler pipeline with the following stages:

1. **Lexical Analysis**: Converts source code into tokens
2. **Parsing**: Builds an Abstract Syntax Tree (AST) from tokens
3. **Semantic Analysis**: Analyses the AST for semantic correctness
4. **Code Generation**: Generates output code from the AST

## Directory Structure

```
argon/
├── src/                  # Source code
│   ├── ast/              # Abstract Syntax Tree definitions
│   ├── lexer/            # Lexical analysis
│   ├── parser/           # Parser implementation
│   ├── token/            # Token definitions
│   ├── utils/            # Utility functions
│   ├── registry/         # Registry for language symbols
│   └── main.cxx          # Main entry point
├── examples/             # Example Argon programs
├── docs/                 # Documentation
├── CMakeLists.txt        # Main CMake configuration
```

## Component Descriptions

### Main Entry Point (`main.cxx`)

The main entry point handles:
- Command-line argument parsing
- File I/O operations
- Orchestrating the compilation pipeline

### Lexer (`lexer/`)

The lexer is responsible for:
- Breaking source code into tokens
- Identifying keywords, identifiers, literals, and operators
- Handling source code positions for error reporting

### Token (`token/`)

The token component defines:
- Token types (keywords, identifiers, literals, etc.)
- Token data structure with lexeme, position, and type information

### Parser (`parser/`)

The parser is responsible for:
- Converting tokens into an Abstract Syntax Tree (AST)
- Implementing grammar rules for the Argon language
- Error reporting for syntax errors

### Abstract Syntax Tree (`ast/`)

The AST component includes:
- Node definitions for different language constructs
- Visitor pattern implementation for traversing the AST
- Base classes for different types of nodes (expressions, statements, etc.)

### Registry (`registry/`)

The registry component handles:
- Symbol table management
- Type checking and semantic analysis
- Scope management

### Utilities (`utils/`)

The utilities component provides:
- File reading and writing functionality
- Helper functions used throughout the compiler

## Compilation Pipeline

1. Source code is read from a file
2. Lexer converts source code to tokens
3. Parser converts tokens to an AST
4. AST is traversed by a visitor to generate output
5. Output is written to a file

## Language Features

The Argon language currently supports:
- Module declarations
- Function definitions with return types
- Variable declarations with types
- Basic expressions (arithmetic, comparison, ternary)
- Return statements

## Future Enhancements

Potential areas for future development:
- Optimisation passes
- More comprehensive type checking
- Standard library implementation
- Backend code generation for different targets
- LLVM integration
