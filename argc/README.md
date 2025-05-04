# Argon Compiler Toolchain Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Building Argon](#building-argon)
4. [Command-Line Usage](#command-line-usage)
5. [Project Structure](#project-structure)
6. [Compiler Phases](#compiler-phases)
   - [Lexical Analysis](#lexical-analysis)
   - [Parsing](#parsing)
   - [Abstract Syntax Tree (AST)](#abstract-syntax-tree-ast)
   - [Symbol Collection](#symbol-collection)
   - [Semantic Analysis](#semantic-analysis)
   - [Type Checking](#type-checking)
   - [Code Generation (x86_64)](#code-generation-x86_64)
7. [Error Handling & Logging](#error-handling--logging)
8. [Configuration & Options](#configuration--options)
9. [Extending Argc](#extending-argc)
10. [Testing](#testing)

---

## Introduction

Argon Compiler (argc) is the compiler for Argon. It demonstrates the core stages of compilation — from lexing and parsing through semantic and type analysis to final code generation — and is designed for ease of extension and experimentation.

## Prerequisites

Before building Argon, ensure you have installed:

- A C++23-capable compiler (e.g. GCC 13+, Clang 16+)
- Flex (for lexical analysis)
- Bison (for parsing)
- LLVM (>= 17) and its development headers
- GNU Make
- Standard development tools (e.g. `git`, `build-essential`)

## Building Argon

1. Clone the repository:
   ```bash
   git clone https://github.com/yasufadhili/argon.git
   cd argon/argc
   ```
2. Build using Make:
   ```bash
   make
   ```
3. The compiler binary will be at `build/bin/argc`.

To clean build artefacts:
```bash
make clean
```  
To perform a very clean removal (including generated parser/lexer):
```bash
make distclean
```  

## Command-Line Usage

```bash
build/bin/argc [options] <input-file>
```

Available options:

- `-h, --help`       Display usage information
- `-v, --version`    Show version and LLVM configuration
- `--debug`          Enable parser/lexer debug traces
- `--verbose`        Print internal AST structure during compilation

On success, Argon emits an x86_64 assembly file with the same base name and a `.s` extension.

## Compiler Phases

Argon’s compilation flow is organised into well-defined stages. Each stage corresponds to a visitor in the `ast` namespace and a phase in `main.cc`.

### Lexical Analysis

- **Tool**: Flex (`src/lexer.l`)  
- **Output**: Stream of tokens with locations  
- **Responsibilities**:
  - Tokenise identifiers, literals, operators, keywords
  - Track file inclusion and line/column positions
  - Classify `IDENT` vs `TYPE_IDENT` using the symbol table

### Parsing

- **Tool**: Bison (`src/parser.yy`)  
- **Output**: Abstract Syntax Tree (AST)
- **Responsibilities**:
  - Grammar for modules, functions, statements, expressions
  - Error recovery with detailed diagnostics
  - Build `ast::unit::TranslationUnit` containing modules

### Abstract Syntax Tree (AST)

- **Definition**: `include/ast.hh`  
- **Nodes**: `TranslationUnit`, `Module`, `Function`, `Statement`, `Expression`, etc.
- **Visitors**:
  - `Printer`: Print tree structure
  - `SymbolCollector`: Gather declarations
  - `TypeChecker`: Enforce type rules and promotions
  - `SemanticAnalyser`: Validate usage, return statements
  - `x86_64_CodeGenerator`: Emit final assembly

### Symbol Collection

- **Visitor**: `ast::SymbolCollector`  
- **Location**: `src/symbol_collector.cc`
- **Responsibilities**:
  - Enter/exit scopes: global → module → function → block
  - Add symbols for modules, functions, variables, parameters
  - Detect duplicate declarations and undeclared uses

### Semantic Analysis

- **Visitor**: `ast::SemanticAnalyser`  
- **Location**: `src/semantic_analyser.cc`
- **Responsibilities**:
  - Verify correct use of return statements
  - Check assignments against `const` and existence
  - Enforce implicit casts via the symbol table’s rules

### Type Checking

- **Visitor**: `ast::TypeChecker`  
- **Location**: `src/type_checker.cc`
- **Responsibilities**:
  - Determine expression types (numeric, pointer, string etc.)
  - Apply binary/unary operator rules and type promotions
  - Report precise diagnostics on type errors

### Code Generation (x86_64)

- **Visitor**: `ast::x86_64_CodeGenerator`  
- **Location**: `include/x86_64_codegen.hh`, `src/x86_64_codegen.cc`
- **Responsibilities**:
  - Manage stack frame layout and variable offsets
  - Translate expressions/statements to AT&T-syntax assembly
  - Handle control flow labels, procedure prologue/epilogue

## Error Handling & Logging

- **Logger**: `logger::Logger` (singleton) with sinks (console/file)  
- **DiagnosticHandler**: Collects parse, semantic, type errors  
- **Usage**: Use `LOG_ERROR`, `LOG_FATAL`, `error::DiagnosticHandler::instance().error(...)` in visitors

## Configuration & Options

- **Config**: `config::Config` holds flags from `driver::OptionParser`
- **Options**:
  - Input files list
  - `show_help`, `show_version`, `debug`, `verbose`
- **Driver**: `src/main.cc` orchestrates phases according to config

## Extending Argc

To add a new feature or phase:

1. **Define AST Node** (if needed) in `include/ast.hh`
2. **Extend Grammar** in `src/parser.yy` and regenerate parser (`make`)
3. **Update SymbolCollector** (if declaration-level) in `src/symbol_collector.cc`
4. **Add Type Rules** in `src/type_checker.cc`
5. **Implement Semantic Checks** in `src/semantic_analyser.cc`
6. **Emit Code** in `src/x86_64_codegen.cc`

Consider writing unit tests and updating the documentation accordingly.

## Testing

- Currently, Argon has no formal test suite. You can write integration tests by compiling source snippets:
  ```bash
  echo "module Main; fn main() i64 { ret 42; }" > test.arg
  build/bin/argc test.arg && gcc test.s -o test
  ./test  # Should exit with code 42
  ```
