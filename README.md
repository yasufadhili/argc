# Argon Programming Language Compiler

A simple compiler for the Argon programming language, designed for simplicity, education, and fun.

## Project Overview

Argon is a statically-typed programming language with a clean syntax and direct x86-64 mapping. This repository contains the compiler implementation, which includes:

- Lexical analysis (tokenisation)
- Parsing and AST generation
- Code generation

## Project Structure

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
├── CMakeLists.txt       # Main CMake configuration
```

## Building from Source

### Prerequisites

- CMake 3.20 or higher
- C++23 compatible compiler (GCC 11+, Clang 14+)
- Git (for cloning the repository)

### Build Instructions

1. Clone the repository:

```bash
git clone https://github.com/yasufadhili/argon.git
cd argon
```

2. Build the project:

```bash
./build.sh
```

### Build Script Options

The `build.sh` script provides several options to customize the build process:

```bash
./build.sh [options]
```

Available options:

- `--release`: Build in Release mode (default is Debug)
- `--clean`: Clean build directory before building
- `--examples`: Build and run examples after building
- `--install`: Install after building
- `--package`: Create packages after building
- `--build-dir DIR`: Set build directory (default is 'build')
- `--help`: Show help message

Examples:

```bash
# Clean build in debug mode
./build.sh --clean

# Build in release mode and run examples
./build.sh --release --examples

# Build, install, and create packages
./build.sh --install --package
```

## Usage

After building, the Argon compiler can be found in the `build/bin` directory. To compile an Argon program:

```bash
./build/bin/argon input_file.ar [output_file]
```

If no output file is specified, the compiler will write to a file named `output` in the current directory.

### Example

```bash
./build/bin/argon examples/3_var.ar my_output
```

## Running Examples

You can run examples in two ways:

### Using the build script:

```bash
./build.sh --examples
```

### Using make commands:

```bash
# Run all examples
cd build
make run_examples

# Run a specific example
cd build
make run_example_3_var  # Runs the 3_var.ar example
```

## License

This project is licensed under the terms of the license included in the repository.

## Contributing

Since this is a toy language, I don't really care much ^:)
