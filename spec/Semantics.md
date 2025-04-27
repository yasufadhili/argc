# Argon Language Semantics

## Table of Contents
1. [Type System](#type-system)
2. [Memory Model](#memory-model)
3. [Execution Model](#execution-model)
4. [Expression Evaluation](#expression-evaluation)
5. [Control Flow](#control-flow)
6. [Function Semantics](#function-semantics)
7. [Module System](#module-system)
8. [Error Handling](#error-handling)
9. [Concurrency Model](#concurrency-model)

## Type System

### Type Categories

Argon's type system consists of several categories:

1. **Primitive Types**
   - Integer types: `i8`, `i16`, `i32`, `i64`, `u8`, `u16`, `u32`, `u64`
   - Floating-point types: `f32`, `f64`
   - Boolean type: `bool`
   - Character type: `rune`
   - String type: `str`

2. **Composite Types**
   - Arrays: `[N]T` (fixed-size) and `[]T` (compiler-determined size)
   - Structures: `struct { ... }`
   - Enumerations: `enum { ... }`

3. **Reference Types**
   - Pointers: `*T`
   - Function types: `def(T) R`

### Type Rules

1. **Type Inference**
   - Variables must have declared types
   - Function can have no return types or even several
   - Array sizes must be compile-time constants

2. **Type Compatibility**
   - Integer types can be implicitly converted to larger integer types
   - Floating-point types can be implicitly converted to larger floating-point types
   - No implicit conversion between integer and floating-point types
   - No implicit conversion between signed and unsigned types

3. **Type Safety**
   - All type conversions must be explicit
   - Array bounds are checked at runtime
   - Null pointer dereferences are caught at runtime
   - Type assertions are checked at runtime

## Memory Model

### Memory Layout

1. **Stack Allocation**
   - Local variables are allocated on the stack
   - Stack grows downward
   - Stack frames include:
     - Return address
     - Saved registers
     - Local variables
     - Function parameters

2. **Heap Allocation**
   - Dynamic data structures are allocated on the heap
   - Managed by the runtime system
   - Garbage collection is not implemented (manual memory management)

3. **Data Alignment**
   - All data types are naturally aligned
   - i8: 1-byte alignment
   - i16: 2-byte alignment
   - i32: 4-byte alignment
   - i64: 8-byte alignment
   - f32: 4-byte alignment
   - f64: 8-byte alignment

### Memory Operations

1. **Variable Declaration**
   ```argon
   var x i32        // Uninitialized, zero value
   var y i32 = 42   // Initialized
   const Z = 100    // Compile-time constant
   ```

2. **Pointer Operations**
   ```argon
   var x i32 = 42
   var p *i32 = &x  // Address-of operator
   *p = 100         // Dereference
   ```

3. **Array Operations**
   ```argon
   var arr [3]i32
   arr[0] = 1       // Bounds checked
   arr[3] = 4       // Runtime error
   ```

## Execution Model

### Program Initialisation

1. **Entry Point**
   - Program execution begins at the `main` function
   - `main` must be declared in the `main` module
   - Command-line arguments are passed to `main`

2. **Module Initialisation**
   - Modules are initialised in dependency order
   - Package-level variables are initialised before function execution
   - Initialisation is single-threaded

### Runtime Environment

1. **System Calls**
   - System calls are made through the standard library
   - File operations, network operations, etc.
   - Error handling through return values

2. **Signal Handling**
   - SIGSEGV: Memory access violations
   - SIGILL: Illegal instructions
   - SIGFPE: Floating-point exceptions

## Expression Evaluation

### Evaluation Order

1. **Operator Precedence**
   ```argon
   // Highest to lowest
   () [] .          // Parentheses, array indexing, member access
   ! ~ ++ --        // Unary operators
   * / %            // Multiplicative
   + -              // Additive
   << >>            // Bitwise shift
   < <= > >=        // Relational
   == !=            // Equality
   &                // Bitwise AND
   ^                // Bitwise XOR
   |                // Bitwise OR
   &&               // Logical AND
   ||               // Logical OR
   = += -= *= /=    // Assignment
   ```

2. **Short-Circuit Evaluation**
   - Logical AND (`&&`): Second operand not evaluated if first is false
   - Logical OR (`||`): Second operand not evaluated if first is true

### Side Effects

1. **Variable Modification**
   - Assignment operations
   - Increment/decrement operators
   - Function calls with reference parameters

2. **I/O Operations**
   - Console input/output
   - File operations
   - Network operations

## Control Flow

### Branching

1. **If Statement**
   ```argon
   if condition {
       // Executed if condition is true
   } else if condition2 {
       // Executed if condition2 is true
   } else {
       // Executed if all conditions are false
   }
   ```

2. **Match Statement**
   ```argon
   match value {
       case: pattern1 {
           // Executed if value matches pattern1
       }
       case: pattern2 {
           // Executed if value matches pattern2
       }
   }
   ```

### Looping

1. **For Loop**
   ```argon
   var i i8 = 0
   for i < 5 {
       // Loop body
       i++
   }
   ```

2. **Repeat**
    ```
    repeat {
      // Loops forever
    }
    ```

    ```
    repeat x {
      // Repeats x number of times
    }

    ```

3. **Repeat While Loop**
   ```argon
   var x = 1
   repeat {
       // Loop body
       x += 1
   } while x < 5
   ```

## Function Semantics

### Function Calls

1. **Parameter Passing**
   - All parameters are passed by value
   - Pointers can be used for pass-by-reference
   - Return values are passed in registers (System V ABI)

2. **Stack Frame**
   ```argon
   // Function prologue
   push rbp
   mov rbp, rsp
   sub rsp, <local_vars_size>

   // Function body
   ...

   // Function epilogue
   mov rsp, rbp
   pop rbp
   ret
   ```

### Function Types

1. **First-Class Functions**
   ```argon
   type BinaryOp func(i32, i32) i32

   def add(a i32, b i32) i32 {
       return a + b
   }

   var operation BinaryOp = add
   ```

2. **Closures**
   - Not supported (no heap allocation for closures)
   - Functions cannot capture variables from outer scope

## Module System

### Module Resolution

1. **Import Paths**
   - Relative to current module
   - Absolute from workspace root
   - No circular dependencies allowed

2. **Symbol Visibility**
   - Exported symbols start with uppercase letter
   - Unexported symbols start with lowercase letter
   - Module-private symbols are not accessible outside module

### Module Initialisation

1. **Dependency Order**
   - Modules are initialized in topological order
   - Circular dependencies are detected at compile time
   - Initialisation is single-threaded

## Error Handling

### Error Types

1. **Runtime Errors**
   - Division by zero
   - Null pointer dereference
   - Array bounds violation
   - Stack overflow

2. **Recoverable Errors**
   ```argon
   def canThrow() throws str {
       // Function that can throw errors
   }
   ```

### Error Propagation

1. **Error Handling**
   - Errors must be explicitly handled
   - No automatic error propagation
   - Error values must be checked

## Concurrency Model

### Threading

1. **Thread Creation**
   - Not supported in current version
   - Future versions may add threading support

2. **Synchronisation**
   - Not supported in current version
   - Future versions may add synchronisation primitives

### Atomic Operations

1. **Atomic Types**
   - Not supported in current version
   - Future versions may add atomic operations

## Notes on x86_64 Implementation

### Register Usage

1. **General Purpose Registers**
   - RAX: Return value
   - RBX: Callee-saved
   - RCX: 4th argument
   - RDX: 3rd argument
   - RSI: 2nd argument
   - RDI: 1st argument
   - RSP: Stack pointer
   - RBP: Frame pointer

2. **Floating-Point Registers**
   - XMM0-XMM7: Arguments and return values
   - XMM8-XMM15: Temporary storage

### Calling Convention

1. **System V AMD64 ABI**
   - First 6 integer arguments in registers
   - First 8 floating-point arguments in XMM registers
   - Additional arguments on stack
   - Return values in RAX/XMM0

2. **Stack Frame**
   - 16-byte aligned
   - Red zone (128 bytes below RSP)
   - Shadow space for register arguments
