# Argon Programming Language Specification


> Argon is a toy programming language designed for educational purposes, It wouldn't be encouraged to take it seriously unless for recreational purposes

## Introduction

Argon is a statically typed, compiled toy programming language designed with modularity, some safety, and curiosity as its core principles. Conceived as an educational project, Argon serves as a vehicle for exploring compiler design, language semantics, and systems programming concepts while remaining accessible to those learning these domains.

The language is currently being implemented for x86-64 architecture on Linux systems, though the specification is designed with potential cross-platform compatibility in mind for future expansion.

### Design Philosophy

Argon embraces several key philosophies that guide its design:

1. **Explicitness over implicitness**: Operations with potentially significant consequences are made explicit in the syntax, reducing unexpected behaviour and increasing code clarity.

2. **Safety without sacrifice**: Memory and type safety mechanisms are built into the language while maintaining performance comparable to other toy systems languages.

3. **Modular by design**: The language encourages composition through a module system that facilitates code organisation and reuse.

4. **Pragmatic minimalism**: While providing the necessary features for systems programming, Argon avoids feature bloat by including only those capabilities essential to its purpose.

5. **Educational transparency**: As a toy language intended for learning, Argon's implementation is designed to be understandable, with straightforward mappings between language features and their compiled representations.

### Target Applications

Argon is particularly well-suited for:

- Educational projects exploring systems programming concepts
- Small to medium-sized systems applications
- Embedded systems development (Running Linux)
- Performance-critical application components
- Projects requiring direct memory control

While initially conceived as a teaching tool, Argon's design principles make it theoretically suitable for production use in constrained environments, though this remains a secondary consideration to its educational value.

### Language Overview

Argon combines elements from established systems programming languages with novel approaches to create a coherent and expressive programming environment:

- **Static typing** with type inference where unambiguous
- **Manual memory management** with optional safety checks
- **First-class modules** for code organisation
- **Pattern matching** for expressive control flow
- **Algebraic data types** for composing complex data structures
- **Controlled mutability** through explicit annotations
- **Deterministic resource management** patterns

The syntax draws inspiration from multiple language traditions while maintaining consistency and readability, with a focus on reducing cognitive overhead for programmers.

## Basic Structure

An Argon source file constitutes a module and follows this general structure:

```rs
// Module declaration
module example;

// Import statements
import std.io;
import std.mem;
import sys

// Type definitions
type Point struct {
  x i32,
  y i32
};

// Constants
const PI: f64 = 3.14159265359;

// Function declarations
fn calculate_distance(a *Point, b *Point) f64 {
  // Function body
  // ...
}

// Entry point
entry {
  // Program entry point
  sys::exit(0);
}
```

## Compiler Implementation

The reference implementation of Argon consists of:

1. A frontend parser that produces an abstract syntax tree
2. A semantic analyser that performs type checking and validation
3. An intermediate representation generator
4. A backend code generator targeting x86-64 assembly
5. A linker interface for producing executable binaries

The compiler is designed to produce machine code with predictable performance characteristics, avoiding hidden costs or abstractions that might impede understanding of the relationship between source code and executable behaviour.

## Specification Structure

This specification is organised into the following sections:

1. **Introduction** (current section)
2. [Type System](./type-system.md)
   - Primitive types
   - Composite types
   - Type inference
   - Type safety guarantees
3. [Lexical Structure](./lexical-structure.md)
   - Keywords
   - Operators
   - Identifiers
   - Literals
4. [Expressions and Statements](./expressions-statements.md)
   - Expression evaluation
   - Statement semantics
   - Control flow
5. [Functions and Procedures](./functions.md)
   - Declaration syntax
   - Parameter passing
   - Return values
   - Inlining and optimisation
6. [Memory Model](./memory-model.md)
   - Memory layout
   - Manual memory management
   - Pointers and references
   - Optional safety features
7. [Module System](./modules.md)
   - Module declaration
   - Imports and exports
   - Visibility rules
8. [Standard Library](./standard-library.md)
   - Core types
   - I/O facilities
   - Memory management
   - Collections
9. [Error Handling](./error-handling.md)
   - Error types
   - Propagation mechanisms
   - Recovery patterns
10. [Concurrency Model](./concurrency.md)
    - Threading primitives
    - Synchronisation mechanisms
    - Message passing
11. [Foreign Function Interface](./ffi.md)
    - C ABI compatibility
    - External function declarations
    - Data representation
12. [Building and Linking](./build-system.md)
    - Compilation model
    - Linker integration
    - Binary format
13. [References](./references)

## Current Status and Roadmap

A reference argon compiler (argc) is currently in an early development stage with the following components implemented:

- Core language parser
- Basic type system
- x86-64 code generation for fundamental constructs

Future development will focus on:

1. Expanding the standard library
2. Implementing advanced type system features
3. Optimising code generation
4. Improving error reporting and diagnostics
5. Developing tooling support (formatter, documentation generator)
6. Exploring additional target platforms

## Community and Contributions

As an educational project, Argon welcomes contributions from those interested in language design, compiler construction, and systems programming. The development process prioritises clarity and learning opportunities over rapid feature development or production optimisation.

Contributors are encouraged to explore and document the rationale behind language design decisions, creating valuable learning resources for others in the process.

---

*This specification is a living document and will evolve as the Argon language matures. It represents the authoritative definition of the language semantics and expected implementation behaviour.*

