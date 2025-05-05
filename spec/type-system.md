# Type System

## Table of Contents

1. [Overview](#overview)
2. [Type Categories](#type-categories)
3. [Primitive Types](#primitive-types)
   - [Integer Types](#integer-types)
   - [Floating-Point Types](#floating-point-types)
   - [Boolean Type](#boolean-type)
   - [Character Type](#character-type)
   - [String Type](#string-type)
   - [Void Type](#void-type)
4. [Composite Types](#composite-types)
   - [Arrays](#arrays)
   - [Slices](#slices)
   - [Tuples](#tuples)
   - [Records](#records)
   - [Enumerations](#enumerations)
   - [Algebraic Data Types](#algebraic-data-types)
5. [Pointer Types](#pointer-types)
   - [Raw Pointers](#raw-pointers)
   - [References](#references)
   - [Memory Safety](#memory-safety)
6. [Error Handling and Reporting](#error-handling-and-reporting)
   - [Error Categories](#error-categories)
   - [Error Message Format](#error-message-format)
   - [Type Errors](#type-errors)
   - [Syntax Errors](#syntax-errors)
   - [Memory Errors](#memory-errors)
   - [Name Resolution Errors](#name-resolution-errors)
   - [Compiler Diagnostics](#compiler-diagnostics)
   - [Runtime Error Handling](#runtime-error-handling)
7. [Type Checking and Verification](#type-checking-and-verification)
8. [Memory Layout and Alignment](#memory-layout-and-alignment)
9. [Type System Extensions](#type-system-extensions)
10. [References and Standards](#references-and-standards)


## Overview

Argon features a strong, static type system designed to catch errors at compile time while providing the expressive power needed for systems programming. This comprehensive type system serves as the foundation for ensuring adequate memory safety, facilitating optimisations, and enabling clear expression of program semantics.

The type system enforces explicit handling of potentially unsafe operations while providing mechanisms for abstracting over types where appropriate. It strikes a balance between safety and flexibility, recognising that systems programming often requires precise control over data representation and memory layout.


## Type Categories

Argon's type system categorises types as follows:

1. **Primitive types**: Basic building blocks representing simple values
2. **Composite types**: Types composed of other types
3. **Pointer types**: Types that represent memory addresses
4. **Function types**: Types representing callable code units
5. **Generic types**: Type templates with parameters

Each category has distinct characteristics and rules governing their use, composition, and conversion.

## Primitive Types

Primitive types represent fundamental values that cannot be decomposed into simpler components. They correspond directly to common hardware-supported types and serve as the atomic units of Argon's type system.

### Integer Types

Argon provides signed and unsigned integer types of various bit widths, following standard two's complement representation:

| Type | Size (bits) | Range | Description |
|------|-------------|-------|-------------|
| `i8` | 8 | -128 to 127 | Signed 8-bit integer |
| `i16` | 16 | -32,768 to 32,767 | Signed 16-bit integer |
| `i32` | 32 | -2,147,483,648 to 2,147,483,647 | Signed 32-bit integer |
| `i64` | 64 | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 | Signed 64-bit integer |
| `u8` | 8 | 0 to 255 | Unsigned 8-bit integer |
| `u16` | 16 | 0 to 65,535 | Unsigned 16-bit integer |
| `u32` | 32 | 0 to 4,294,967,295 | Unsigned 32-bit integer |
| `u64` | 64 | 0 to 18,446,744,073,709,551,615 | Unsigned 64-bit integer |
| `isize` | Architecture-dependent | Platform word size (signed) | Platform-dependent signed integer |
| `usize` | Architecture-dependent | Platform word size (unsigned) | Platform-dependent unsigned integer |

Integer literals can be specified in decimal, hexadecimal, octal, or binary notation:

```rust
// Integer literals
var decimal i32 = 42;
var hex i32 = 0x2A;
var octal i32 = 0o52;
var binary i32 = 0b101010;
```

Integer operations that would result in overflow are considered errors in debug mode and exhibit defined wrap-around behaviour in release mode. This can be controlled with explicit overflow-checking operations:

```rust
// Soon
```

### Floating-Point Types

Argon implements IEEE 754 floating-point types:

| Type | Size (bits) | Precision | Description |
|------|-------------|-----------|-------------|
| `f32` | 32 | Single precision | IEEE 754-2008 binary32 |
| `f64` | 64 | Double precision | IEEE 754-2008 binary64 |

Floating-point literals can include decimal points and exponents:

```rust
// Floating-point literals
var pi f64 = 3.14159;
var avogadro f64 = 6.022e23;
```

Argon fully supports special floating-point values including `NaN` (Not a Number), positive and negative infinity, and signed zero as defined by IEEE 754.

### Boolean Type

The `bool` type represents a logical value that can be either `true` or `false`:

| Type | Size (bits) | Values | Description |
|------|-------------|--------|-------------|
| `bool` | 8 | `true`, `false` | Logical boolean value |

Boolean operations include logical AND (`&&`), logical OR (`||`), and logical NOT (`!`).

```rust
var is_valid bool = true;
var has_error bool = false;
var combined bool = is_valid && !has_error;
```

### Character Type

The `char` type represents a single Unicode scalar value:

| Type | Size (bits) | Range | Description |
|------|-------------|-------|-------------|
| `char` | 32 | U+0000 to U+D7FF and U+E000 to U+10FFFF | Unicode scalar value |

Character literals are enclosed in single quotes:

```rs
var a char = 'a';
var lambda char = 'λ';
var emoji char = '😊';
```

The `char` type excludes surrogate code points (U+D800 to U+DFFF) in accordance with Unicode standards.

### String Type

While not strictly a primitive type, strings are fundamental enough to warrant discussion here. Argon represents strings as UTF-8 encoded sequences of bytes:

| Type | Implementation | Encoding | Description |
|------|---------------|----------|-------------|
| `str` | Fat pointer (ptr + len) | UTF-8 | Immutable string reference |
| `Str` | Heap-allocated buffer | UTF-8 | Mutable, owned string |

String literals are enclosed in double quotes:

```rs
var greeting str = "Hello, world!";
var multilingual str = "你好, こんにちは, नमस्ते";
```

Escape sequences in strings follow C-like conventions with extensions for Unicode:

```rs
var escaped str = "Line 1\nLine 2\tTabbed";
var unicode str = "\u{1F642}"; // 🙂
```

Raw string literals, which do not process escape sequences, are prefixed with `r`:

```rs
var raw str = r"C:\Program Files\Argon";
```
### Void Type

The `void` type represents the absence of a value:

| Type | Size (bits) | Values | Description |
|------|-------------|--------|-------------|
| `void` | 0 | None | Absence of value |

This type is primarily used as the return type for functions that do not return a value:

```rs
fn log_message(msg str) void {
  // Function that returns nothing
}
```

## Composite Types

Composite types are constructed from other types and provide ways to group related data together.

### Arrays

Arrays represent fixed-size, homogeneous collections of elements stored contiguously in memory:

```rs
// Array type syntax: [N]T where T is the element type and N is the length
var fibonacci [5] i32 = {1, 1, 2, 3, 5};
```

Arrays have the following properties:

- Fixed size determined at compile time
- Elements must be of the same type
- Zero-indexed without bounds checking
- Contiguous memory layout

Operations on arrays include indexing, slicing, and iteration:

```rs
// Indexing
var third i32 = fibonacci[2]; // 2

// Slicing
var subset &[]i32 = &fibonacci[1..4]; // [1, 2, 3]
```

### Slices

Slices represent views into arrays or other contiguous sequences:

```rs
// Slice type syntax: &[]T
fn sum_values(values &[]i32) i32 {
  // Calculate sum of values in slice
}
```

Slices have the following properties:

- Dynamic size determined at runtime
- Implemented as a pointer and a length
- Can view into arrays, vectors, or other slices
- Support for bounds checking



### Tuples

Tuples are heterogeneous collections of fixed size:

```rs
// Tuple type syntax: (T1, T2, ..., Tn)
var point (i32, i32) = (10, 20);
var my_record (str, u32, bool) = ("Alice", 30, true);
```

Tuples have the following properties:

- Fixed size determined at compile time
- Elements can be of different types
- Elements accessed by index or through destructuring
- No named fields (use records for that)

```rs
// Access by index
var x = point.0; // 10

// Destructuring
var (name, age, active) = my_record;
```

### Records

Records represent named collections of fields:

```rs
// Record definition
type Point record {
  x i32,
  y i32
};

// Structure instantiation
var origin = Point { x: 0, y: 0 };
```

Records have the following properties:

- Named fields with specified types
- Field access using dot notation
- Can be made generic over types
- Support for methods through implementations

```rs
// Implementations

fn new (x i32, y i32) Point {
  ret Point { x, y };
}

fn (p Point) distance_from_origin() f64 {
  ret sqr((p.x * p.x + p.y * p.y) as f64);
}

```


### Enumerations

Enumerations represent values that can be one of several variants:

```rs
// Enumeration definition
type Colour enum {
  Red,
  Green,
  Blue,
  RGB(u8, u8, u8)
};

// Enumeration usage
var primary = Colour::Red;
var custom = Colour::RGB(128, 64, 32);
```

Enumerations have the following properties:

- Each variant may have an optional payload
- Pattern matching for control flow based on variants
- Efficient representation based on the variants

```rs
// Pattern matching on enumerations
match colour {
  case Colour::Red {
    std::println("It's red");
  }
  case Colour::Green { 
    std::println("It's green"); 
  }
  case Colour::Blue {
    std::println("It's blue");
  }
  case Colour::RGB(r, g, b) {
    std::printf("RGB: %d, %d, %d", r, g, b);
  }
}
```

### Algebraic Data Types

Argon supports algebraic data types through a combination of enumerations and structures, providing powerful tools for modelling complex domains:

```rs
// Sum type (enum with variants)
type Shape enum {
  Circle(Circle),
  Rectangle(Rectangle),
  Triangle(Triangle)
};

// Product types (records with fields)
type Circle record {
  center Point,
  radius f64
};

type Rectangle record {
  top_left Point,
  bottom_right Point
};

type Triangle record {
  a Point,
  b Point,
  c Point
};
```


## Pointer Types

Argon provides a straightforward approach to pointers that emphasizes simplicity and directness while maintaining educational value.

### Raw Pointers

Basic pointers in Argon represent memory addresses, allowing for direct memory manipulation:

```rs
// Pointer type syntax: *T (pointer to type T)
var int_value i32 = 42;
var ptr *i32 = &int_value;  // Take address with &
```

Basic pointers have the following properties:

- Store the memory address of a value
- Can be dereferenced to access the underlying value
- Support pointer arithmetic
- Can be null (represented by the literal `null`)
- Size is always 8 bytes (since argon only supports 64bit systems)

```rs
// Pointer operations
var x i32 = 10;
var p *i32 = &x;           // Address-of operator
var y i32 = *p;            // Dereference operator
p = null;                  // Null assignment
```

### Mutable and Immutable Pointers

Argon distinguishes between pointers to mutable and immutable data:

```rs
// Immutable pointer (default)
var value i32 = 42;
var ptr *i32 = &value;     // Points to immutable data
// *ptr = 100;             // Error: Cannot modify through immutable pointer

// Mutable pointer
var mutable i32 = 42;
var mut_ptr *mut i32 = &mutable;  // Points to mutable data
*mut_ptr = 100;            // Valid: Can modify through mutable pointer
```

This distinction helps prevent accidental modifications while still allowing for them when necessary.

### References

Argon provides references as a safer alternative to raw pointers:

```rs
// Reference type syntax: &T (reference to type T)
var value i32 = 42;
var ref &i32 = &value;  // Create reference
var x i32 = *ref;       // Dereference (implicit in most contexts)
```

References have the following characteristics:
- Cannot be null
- Cannot be reseated (changed to point to something else)
- Automatically dereferenced in most contexts
- No pointer arithmetic allowed

References are preferred for most use cases where pointers are needed.

### Memory Safety

Argon employs several mechanisms to help ensure memory safety:

1. **Null checking**: The compiler can insert runtime checks for null pointers
2. **Reference safety**: References cannot be null and have a defined lifetime
3. **Bounds checking**: Array and slice accesses can be checked at runtime

```rs
// Null checking
var ptr *i32 = null;
// var value i32 = *ptr;  // Runtime error: Null pointer dereference

// Bounds checking
var arr [3]i32 = {1, 2, 3};
// var value i32 = arr[5];  // Runtime error: Index out of bounds
```

While Argon doesn't guarantee memory safety like some modern languages, it provides tools to help programmers write safer code.

## Error Handling and Reporting

Argon provides a comprehensive error handling system designed for clarity, educational value, and practicality. The compiler's error messages aim to be informative and helpful, guiding programmers toward correct solutions.

### Error Categories

Argon classifies errors into several categories:

1. **Type errors**: Issues related to incompatible types
2. **Syntax errors**: Problems with the grammatical structure of code
3. **Memory errors**: Issues related to memory management and safety
4. **Name resolution errors**: Problems finding variables, functions, or types
5. **Constraint violations**: Violations of language rules or constraints
6. **Semantic errors**: Issues with program logic or semantics

### Error Message Format

Argon error messages follow a consistent format:

```
Error [E0001]: Brief description of the error
  --> file.ar:line:column
   |
 L | problematic_code
   |          ^^^^^ Specific indication of the error
   |
   = Note: Additional context or explanation
   = Help: Suggested fix or improvement
```

Each error is assigned a unique code (e.g., E0001) that can be used to look up more detailed documentation.

### Type Errors

Type errors occur when operations are performed on incompatible types:

#### Assignment Type Mismatch

```rs
var x i32 = "hello";  // Type error
```

```
Error [E0101]: Type mismatch in variable initialization
  --> main.ar:1:12
   |
 1 | var x i32 = "hello";
   |            ^^^^^^^ Expected type 'i32', found 'str'
   |
   = Help: Consider using a numeric value or explicit conversion
```

#### Function Argument Type Mismatch

```rs
fn add(a i32, b i32) i32 {
  ret a + b;
}

add(5, "hello");  // Type error
```

```
Error [E0102]: Type mismatch in function argument
  --> main.ar:5:8
   |
 5 | add(5, "hello");
   |        ^^^^^^^ Expected type 'i32' for parameter 'b', found 'str'
   |
   = Note: Function 'add' expects 'i32' for its second parameter
   = Help: Consider using a numeric value or explicit conversion
```

#### Operator Type Mismatch

```rs
var a str = "hello";
var b i32 = 5;
var c = a + b;  // Type error
```

```
Error [E0103]: Invalid operands for binary operator '+'
  --> main.ar:3:9
   |
 3 | var c = a + b;
   |         ^ ^ ^ Type 'str' cannot be added to type 'i32'
   |
   = Help: Consider converting one of the operands or using string formatting
```

#### Return Type Mismatch

```rs
fn get_value() i32 {
  ret "hello";  // Type error
}
```

```
Error [E0104]: Type mismatch in return statement
  --> main.ar:2:7
   |
 2 |   ret "hello";
   |       ^^^^^^^ Expected return type 'i32', found 'str'
   |
   = Help: Consider returning a numeric value or changing the function's return type
```

#### Type Cast Error

```rs
var x f64 = 3.14;
var y i32 = x;  // Type error: implicit cast
```

```
Error [E0105]: Cannot implicitly convert between types
  --> main.ar:2:12
   |
 2 | var y i32 = x;
   |            ^ Cannot implicitly convert from 'f64' to 'i32'
   |
   = Help: Use an explicit cast: 'x as i32'
```

### Syntax Errors

Syntax errors occur when code doesn't follow the language's grammatical rules:

#### Missing Semicolon

```rs
var x i32 = 5  // Missing semicolon
var y i32 = 10;
```

```
Error [E0201]: Expected ';' after statement
  --> main.ar:1:14
   |
 1 | var x i32 = 5
   |              ^ Expected ';' here
   |
   = Help: Add a semicolon to end the statement
```

#### Unmatched Brackets

```rs
fn calculate() i32 {
  var x i32 = (5 + 3 * (2 + 1;  // Unmatched parenthesis
  ret x;
}
```

```
Error [E0202]: Unmatched opening parenthesis
  --> main.ar:2:21
   |
 2 |   var x i32 = (5 + 3 * (2 + 1;
   |                     ^ Opening '(' has no matching ')'
   |
   = Help: Add a closing ')' or check for mismatched delimiters
```

#### Invalid Token

```rs
var $ i32 = 5;  // Invalid character for identifier
```

```
Error [E0203]: Invalid token
  --> main.ar:1:5
   |
 1 | var $ i32 = 5;
   |     ^ Invalid character in identifier name
   |
   = Help: Variable names must start with a letter or underscore and can contain letters, numbers, and underscores
```

#### Unexpected Token

```rs
fn test() {
  if (x > 5) {
    ret true;
  } else ret false;  // Missing braces
}
```

```
Error [E0204]: Expected block after 'else'
  --> main.ar:4:9
   |
 4 |   else ret false;
   |         ^^^ Expected '{' here
   |
   = Help: Use braces to create a block: 'else { ret false; }'
```

### Memory Errors

Memory errors relate to issues with memory management and safety:

#### Null Pointer Dereference

```rs
var ptr *i32 = null;
var value i32 = *ptr;  // Null pointer dereference
```

```
Error [E0301]: Possible null pointer dereference
  --> main.ar:2:17
   |
 2 | var value i32 = *ptr;
   |                 ^^^^ Dereferencing a pointer that may be null
   |
   = Note: 'ptr' was assigned 'null' on line 1
   = Help: Check if the pointer is not null before dereferencing: 'if (ptr != null) { ... }'
```

#### Use After Free

```rs
fn use_after_free() {
  var ptr *i32 = mem::alloc(sizeof(i32)) as *i32;
  *ptr = 42;
  mem::dealloc(ptr);
  var value i32 = *ptr;  // Use after free
}
```

```
Error [E0302]: Potential use after free
  --> main.ar:5:21
   |
 4 |   mem::dealloc(ptr);
   |   ----------------- Memory freed here
 5 |   var value i32 = *ptr;
   |                     ^^^ Dereferencing pointer after memory was deallocated
   |
   = Help: Do not use pointers after calling mem::dealloc on them
```

#### Memory Leak

```rs
fn memory_leak() {
  var ptr *i32 = mem::alloc(sizeof(i32)) as *i32;
  *ptr = 42;
  // No deallocation before function returns
}
```

```
Warning [W0303]: Potential memory leak
  --> main.ar:2:12
   |
 2 |   var ptr *i32 = mem::alloc(sizeof(i32)) as *i32;
   |            ^^^^ Memory allocated here but never freed
   |
   = Help: Call mem::dealloc(ptr) before the end of the function
```

#### Buffer Overflow

```rs
fn buffer_overflow() {
  var array [5]i32 = {1, 2, 3, 4, 5};
  var ptr *i32 = &array[0];
  
  var value i32 = *(ptr + 10);  // Accessing beyond array bounds
}
```

```
Warning [W0304]: Potential buffer overflow
  --> main.ar:5:22
   |
 5 |   var value i32 = *(ptr + 10);
   |                      ^^^^^^^^ Pointer arithmetic may access memory beyond array bounds
   |
   = Note: Array 'array' has 5 elements, but trying to access index equivalent to 10
   = Help: Ensure pointer arithmetic remains within array bounds
```

### Name Resolution Errors

Name resolution errors occur when the compiler cannot find a referenced identifier:

#### Undefined Variable

```rs
fn use_undefined() {
  var x i32 = y;  // Undefined variable 'y'
}
```

```
Error [E0401]: Use of undefined variable
  --> main.ar:2:14
   |
 2 |   var x i32 = y;
   |              ^ Variable 'y' has not been declared
   |
   = Help: Declare 'y' before using it or check for a typo
```

#### Undefined Function

```rs
fn main() {
  calculate();  // Undefined function
}
```

```
Error [E0402]: Call to undefined function
  --> main.ar:2:3
   |
 2 |   calculate();
   |   ^^^^^^^^^ Function 'calculate' has not been declared
   |
   = Help: Define function 'calculate' or check for a typo
```

#### Undefined Type

```rs
fn use_undefined_type() {
  var x MyType = {};  // Undefined type 'MyType'
}
```

```
Error [E0403]: Use of undefined type
  --> main.ar:2:9
   |
 2 |   var x MyType = {};
   |         ^^^^^^ Type 'MyType' has not been declared
   |
   = Help: Define type 'MyType' with 'type MyType record { ... }' or check for a typo
```

#### Duplicate Definition

```rs
fn duplicate() i32 {
  ret 1;
}

fn duplicate() void {  // Duplicate function definition
  // Function body
}
```

```
Error [E0404]: Duplicate definition
  --> main.ar:5:1
   |
 1 | fn duplicate() i32 {
   | --------------------- First definition here
 ...
 5 | fn duplicate() void {
   | ^^^^^^^^^^^^^^^^^^^ Function 'duplicate' already defined
   |
   = Help: Use a different name for this function or remove one of the definitions
```

### Compiler Diagnostics

In addition to errors, Argon provides warnings and notes to help improve code quality:

#### Unused Variable Warning

```rs
fn has_unused() {
  var x i32 = 5;  // Unused variable
  ret 42;
}
```

```
Warning [W0501]: Unused variable
  --> main.ar:2:7
   |
 2 |   var x i32 = 5;
   |       ^ Variable 'x' is never used
   |
   = Help: Remove the unused variable or prefix with '_' to suppress this warning
```

#### Implicit Conversion Note

```rs
fn implicit_conversion() {
  var x i32 = 5;
  var y f64 = x;  // Implicit conversion (safe but noted)
}
```

```
Note [N0502]: Implicit conversion
  --> main.ar:3:14
   |
 3 |   var y f64 = x;
   |              ^ Implicit conversion from 'i32' to 'f64'
   |
   = Help: This conversion is safe but you may want to make it explicit for clarity: 'x as f64'
```

#### Performance Suggestion

```rs
fn performance_issue() {
  var sum i32 = 0;
  var i i32 = 0;
  repeat {
    sum += i * i;  // Redundant computation
    i++;
  } until (i >= 1000);
}
```

```
Note [N0503]: Potential performance improvement
  --> main.ar:5:12
   |
 5 |     sum += i * i;
   |            ^^^^^ Repeated computation in loop
   |
   = Help: Consider storing 'i * i' in a temporary variable outside the loop body
```

### Runtime Error Handling

Argon provides mechanisms for handling runtime errors:

#### Assert Statement

```rs
fn divide(a i32, b i32) i32 {
  assert(b != 0, "Division by zero");
  ret a / b;
}
```

If `b` is zero, the program will terminate with:

```
Runtime Error: Assertion failed: Division by zero
  at divide (main.ar:2:3)
  at main (main.ar:7:14)
```

#### Ensure Statement

```rs
fn get_element(array &[]i32, index usize) i32 {
  ensure(index < len(array), ret -1);
  ret array[index];
}
```

`ensure` provides a way to handle potential errors without terminating the program.

#### Explicit Error Handling

While Argon doesn't have built-in error types like some languages, it encourages explicit error handling patterns:

```rs
// Return result with error code
fn divide(a i32, b i32, result *i32) i32 {
  if (b == 0) {
    ret 1;  // Error code: division by zero
  }
  *result = a / b;
  ret 0;  // Success code
}

// Usage with explicit error handling
fn main() {
  var result i32 = 0;
  var error_code i32 = divide(10, 0, &result);
  
  if (error_code != 0) {
    std::println("Error: Division by zero");
  } else {
    std::printf("Result: %d\n", result);
  }
}
```

## Type Checking and Verification

Argon employs a robust but straightforward type checking system to catch errors at compile time while maintaining a clear mental model for programmers.

### Static Type Checking

Argon performs static type checking during compilation:

```
var x i32 = 5;
var y f64 = 3.14;
x = y;  // Compile error: Cannot assign f64 to i32 without explicit conversion
```

The type checker enforces:

- Type compatibility in assignments and function calls
- Correct use of operators for given types
- Proper implementation of interfaces and traits
- Valid generic type parameter substitutions

### Type Compatibility

Argon defines clear rules for type compatibility:

1. **Identity**: A type is always compatible with itself
2. **Widening**: Smaller numeric types can implicitly convert to larger ones (e.g., `i8` to `i16`)
3. **Reference-to-pointer**: References can implicitly convert to pointers
4. **Array-to-slice**: Arrays can implicitly convert to slices

### Type Assertions

Type assertions allow programmers to explicitly cast between compatible types:

```rs
var f f64 = 3.14;
var i i32 = f as i32;  // Explicit conversion
```

Type assertions are particularly useful when working with interfaces and generic types.

## Memory Layout and Alignment

Argon provides predictable and efficient memory layouts for its types, essential for systems programming.

### Primitive Type Layout

Primitive types have straightforward memory layouts:

| Type | Size (bytes) | Alignment (bytes) | Representation |
|------|--------------|-------------------|----------------|
| `i8` | 1 | 1 | Two's complement |
| `i16` | 2 | 2 | Two's complement |
| `i32` | 4 | 4 | Two's complement |
| `i64` | 8 | 8 | Two's complement |
| `u8` | 1 | 1 | Unsigned binary |
| `u16` | 2 | 2 | Unsigned binary |
| `u32` | 4 | 4 | Unsigned binary |
| `u64` | 8 | 8 | Unsigned binary |
| `f32` | 4 | 4 | IEEE 754 single precision |
| `f64` | 8 | 8 | IEEE 754 double precision |
| `char` | 4 | 4 | Unicode code point |
| `bool` | 1 | 1 | 0 = false, 1 = true |

### Composite Type Layout

Composite types follow these layout rules:

1. **Arrays**: Elements are stored contiguously with no padding between them
2. **Records**: Fields are laid out in declaration order with padding for alignment
3. **Tuples**: Components are laid out in order with padding for alignment
4. **Enumerations**: Size is determined by the largest variant plus a tag

Example of record layout:

```
// Record definition
type Example record {
    a i8,    // 1 byte
    b i32,   // 4 bytes
    c i16    // 2 bytes
};

// Memory layout (64-bit architecture):
// Offset 0:  a (1 byte)
// Offset 1:  padding (3 bytes)
// Offset 4:  b (4 bytes)
// Offset 8:  c (2 bytes)
// Offset 10: padding (2 bytes)
// Total size: 12 bytes
```

### Alignment Control

Argon allows control over alignment through attributes:

```
// Custom alignment
type AlignedData record #[align(16)] {
    value i32
};

// Packed record with no padding
type PackedData record #[packed] {
    a i8,
    b i32,
    c i16
};
```

The `#[align(N)]` attribute sets a minimum alignment, while `#[packed]` removes padding between fields.

### Memory Inspection

Argon provides built-in functions for inspecting memory layout:

```
// Get size and alignment information
var size = sizeof(Example);       // Size in bytes
var alignment = alignof(Example); // Alignment in bytes

// Get field offset
var offset = offsetof(Example, b); // Offset of field b in bytes

std::printf("Size: %zu bytes, Alignment: %zu bytes, Offset of b: %zu bytes\n", 
            size, alignment, offset);
```

These tools are valuable for low-level programming tasks like serialization or FFI.

### Pointer Alignment

Pointers must respect alignment requirements of their target types:

```
var data i32 = 42;
var ptr *i32 = &data;  // Properly aligned pointer

// Misaligned access (undefined behavior)
var bytes *u8 = ptr as *u8;
var misaligned *i32 = (bytes + 1) as *i32;  // Potentially misaligned
```

Proper alignment is essential for correct behavior and performance.

## Type System Extensions

Argon provides several extensions to the core type system that enhance its expressiveness and safety.

### Type Aliases

Type aliases create alternative names for existing types:

```
// Type alias syntax
type Size = usize;
type NodeIndex = u32;
type Point = (i32, i32);

// Usage
var window_size Size = 1024;
var node_id NodeIndex = 42;
var position Point = (10, 20);
```

Aliases improve code readability and make future type changes easier.

### Type Unions

Unions allow a variable to hold different types of values:

```
// Union type syntax
type Value union {
    i32,
    f64,
    str
};

// Usage
var v1 Value = 42;       // Store an i32
var v2 Value = 3.14;     // Store an f64
var v3 Value = "hello";  // Store a str

// Pattern matching is required to safely access union values
match v1 {
    case i: i32 {
        std::printf("Integer: %d\n", i);
    }
    case f: f64 {
        std::printf("Float: %f\n", f);
    }
    case s: str {
        std::printf("String: %s\n", s);
    }
}
```

Unions provide a simple form of sum types without the complexity of full algebraic data types.

# References and Standards

References available in the [References](./references.md) document