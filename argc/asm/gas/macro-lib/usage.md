
## Compilation and Execution

To compile and run these examples, we would use GAS (via gcc) with the following commands:


# Compile a single assembly file
gcc -no-pie -o simple_macro simple_macro.asm

# Compile with external functions (like printf)
gcc -no-pie -o advanced_macros advanced_macros.asm

# Compile the system call example
gcc -no-pie -o syscall_macros syscall_macros.asm

# Compile a program that uses a macro library
gcc -no-pie -o main main.asm

# Execute the compiled program
./simple_macro
./advanced_macros
./syscall_macros
./main

# To define preprocessor symbols for conditional assembly
gcc -no-pie -DUSE_SSE -o opt_main main.asm


# GAS Macros for Code Reuse in x86-64 Linux Assembly

GAS (GNU Assembler) macros are a powerful feature that enables code reuse in assembly programming. Demonstrate how to define and use macros in x86-64 Linux assembly with examples.

## Basic Macro Syntax

In GAS, macros are defined using the `.macro` and `.endm` directives. The general syntax is as follows:

```nasm
.macro name arg1, arg2, ...
    ; macro body with \arg1, \arg2, etc.
.endm
```


## Key Takeaways

1. **Basic Syntax**: GAS macros are defined using `.macro` and `.endm` directives.

2. **Parameter Handling**:
   - Standard parameters use `\param` syntax
   - Optional parameters can have default values
   - Variable arguments are supported via `vararg`

3. **Local Labels**: Use `\@` for local labels within macros to ensure uniqueness.

4. **Recursive Macros**: GAS supports recursive macro expansion, useful for compile-time computations.

5. **Conditional Assembly**: Directives like `.if`, `.ifdef`, `.else`, and `.endif` allow for conditional code generation.

6. **Repeating Blocks**: `.irp` and `.rept` directives facilitate repeating code blocks.

7. **Nested Macros**: Macros can call other macros, enabling complex code generation patterns.

8. **Common Use Cases**:
   - System call wrappers
   - Register preservation
   - String and memory operations
   - Control flow abstraction
   - Build-time optimisations via conditional compilation
   - Table generation

9. **Benefits**:
   - Improved code readability
   - Reduced repetition
   - Consistent error handling
   - Platform abstraction
   - Simplified maintenance
