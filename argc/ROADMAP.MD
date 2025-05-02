# Disorganised Roadmap

1. **Restructure the Type System**:
    - Create a more robust type handling system that can handle compound types
    - Integer types (i8, i16, i32, i64)
    - Unsigned type (u8, u16, u32, u64)
    - Floating point values (f32, f64) - Single and double precision
    - Should handle decimal, binary, octal and hexadecimal
    - Exponents
    - Add support for arrays and custom types

2. **Add Statement Terminators**:
    - Ensure consistent use of semicolons for statement termination
    - The current grammar is inconsistent about where semicolons are required

3. **Add Error Recovery**:
    - Implement more robust error recovery mechanisms in the parser
    - Currently, error handling is minimal with just a simple message

4. **Extend Expression Capabilities**:
    - Add support for ternary operations
    - Add support for compound assignments (+=, -=, etc.)
    - Implement array access expressions

5. **Code Organisation**:
    - Group related rules together for better maintainability
    - Add comprehensive comments documenting each rule's purpose
