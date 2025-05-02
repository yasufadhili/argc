# Numeric Types Specification

## Supported Number Types

### Integer Types
- Unsigned integers: `u8`, `u16`, `u32`, `u64`
- Signed integers: `i8`, `i16`, `i32`, `i64`
- Size guarantees:
  - `u8`: 0 to 255
  - `u16`: 0 to 65,535
  - `u32`: 0 to 4,294,967,295
  - `u64`: 0 to 18,446,744,073,709,551,615
  - `i8`: -128 to 127
  - `i16`: -32,768 to 32,767
  - `i32`: -2,147,483,648 to 2,147,483,647
  - `i64`: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

### Floating-Point Types
- `f32`: IEEE-754 32-bit floating-point
- `f64`: IEEE-754 64-bit floating-point

## Type Conversion Rules

### Implicit Conversions
- Allowed when no precision loss occurs:
  - `u8` → `u16` → `u32` → `u64`
  - `i8` → `i16` → `i32` → `i64`
  - `f32` → `f64`
  - `u8` → `i16` → `i32` → `i64`
  - `u16` → `i32` → `i64`

### Explicit Casting
Must use explicit casting for:
- Any conversion with potential precision loss
- Converting between signed and unsigned types
- Converting from floating-point to integer types
- Converting from larger to smaller types

Syntax: `value as TypeName`

Example:
```argon
var x i32 = 42;
var y f64 = x as f64;
```

## Numeric Operations

### Integer Operations
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- Remainder: `%`
- Bitwise AND: `&`
- Bitwise OR: `|`
- Bitwise XOR: `^`
- Left shift: `<<`
- Right shift: `>>`

### Floating-Point Operations
- Addition: `+`
- Subtraction: `-`
- Multiplication: `*`
- Division: `/`
- No remainder operation

### Operation Rules
1. Operations between same types return the same type
2. Mixed-type operations use the larger type
3. Division between integers results in truncation
4. Integer overflow behavior:
   - Debug: panic on overflow
   - Release: wrap around

## Standard Library Functions

### Required Numeric Type Methods
- `max()`: Maximum value for the type
- `min()`: Minimum value for the type
- `is_positive()`: Returns true if > 0
- `is_negative()`: Returns true if < 0
- `abs()`: Absolute value (signed types only)
- `to_string()`: Convert to string representation

## Error Handling

### Runtime Checks
1. Division by zero
2. Integer overflow (in debug mode)
3. Invalid casting operations

### Compile-time Checks
1. Type compatibility in assignments
2. Constant expression evaluation
3. Range checking for literal values
