Pedagogically ordered list of files for learning x86-64 GAS (AT&T syntax) assembly, starting from the simplest (program exit) to advanced constructs (functions, conditionals, scopes, etc.). Each file builds on the previous, introducing new concepts. The prefix number indicates the learning order.

---

### 1. File List (with Explanations)

1. **01_exit.asm**  
   Minimal program: exits with a status code.

2. **02_hello_world.asm**  
   Print "Hello, World!" using Linux syscalls.

3. **03_mov_add_sub.asm**  
   Demonstrates `mov`, `add`, `sub` instructions.

4. **04_arithmetic.asm**  
   Multiplication, division, increment, decrement.

5. **05_bitwise.asm**  
   Bitwise operations: `and`, `or`, `xor`, `not`, `shl`, `shr`.

6. **06_comparisons.asm**  
   Relational expressions: `cmp`, `test`, flags.

7. **07_conditionals.asm**  
   Conditional jumps: `je`, `jne`, `jg`, `jl`, etc.

8. **08_loops.asm**  
   Loop constructs: `jmp`, `loop`, `jcxz`.

9. **09_arrays.asm**  
   Array access, iteration, and manipulation.

10. **10_functions.asm**  
    Simple function call and return (`call`, `ret`).

11. **11_functions_params.asm**  
    Functions with parameters (using registers, stack).

12. **12_functions_return.asm**  
    Functions with return values.

13. **13_functions_multi_return.asm**  
    Functions returning multiple values (via registers/memory).

14. **14_stack.asm**  
    Stack operations: `push`, `pop`, stack frames.

15. **15_scope_local_vars.asm**  
    Local variables, variable scope within functions.

16. **16_recursion.asm**  
    Recursive function example (e.g., factorial).

17. **17_structs.asm**  
    Simulating structs/records using memory.

18. **18_strings.asm**  
    String manipulation and syscalls.

19. **19_switch_case.asm**  
    Implementing switch/case logic with jump tables.

20. **20_macros.asm**  
    Using GAS macros for code reuse.

21. **21_inline_data.asm**  
    Defining and using inline data (constants, arrays).

22. **22_io.asm**  
    Reading from stdin, writing to stdout.

23. **23_math_library.asm**  
    Calling C math library functions from assembly.

24. **24_error_handling.asm**  
    Error handling and exit codes.

25. **25_high_level_constructs.asm**  
    Simulating high-level constructs (structs, classes, etc.).

---

### 2. Achieving Complex Constructs in Assembly

- **Arithmetic/Bitwise:**  
  Use instructions like `add`, `sub`, `imul`, `idiv`, `and`, `or`, `xor`, `shl`, `shr`.

- **Relational Expressions:**  
  Use `cmp` to set flags, then conditional jumps (`je`, `jne`, etc.) for logic.

- **Functions with Parameters/Return:**  
  Pass parameters via registers (`rdi`, `rsi`, `rdx`, etc. per System V ABI), return values in `rax`, multiple returns via other registers or memory.

- **Conditionals:**  
  Use `cmp` + conditional jumps for `if`, `else`, `switch`.

- **Loops:**  
  Use labels and jumps for `while`, `for`, `do-while`.

- **Scopes/Local Variables:**  
  Allocate space on the stack within function prologues/epilogues.

- **Multiple Return Values:**  
  Use additional registers or memory locations.

- **Structs/Records:**  
  Use contiguous memory and offsets.

- **Recursion:**  
  Standard function call/return, stack for local state.

- **Macros:**  
  GAS supports macros for code reuse.

---
