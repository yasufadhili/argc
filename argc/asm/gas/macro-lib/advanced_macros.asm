.section .data
    fmt_int: .asciz "%d\n"

.section .text
    .global main

# Recursive macro for factorial calculation at assembly time
.macro FACTORIAL n, result=1
    .if \n > 1
        FACTORIAL \n-1, \result*\n
    .else
        .set FACT_RESULT, \result
    .endif
.endm

# Macro for generating a lookup table
.macro GENERATE_SQUARE_TABLE start, end
    .section .data
square_table:
    .irp num, \start, \start+1, \start+2, \start+3, \start+4, \start+5, \start+6, \start+7, \start+8, \start+9
        .long \num*\num
    .endr
.endm

# Macro to generate Fibonacci sequence at assembly time
.macro FIBONACCI count, a=0, b=1
    .long \a
    .if \count > 1
        FIBONACCI \count-1, \b, \a+\b
    .endif
.endm

# Variadic macro for summing values
.macro SUM_REGISTERS result, regs:vararg
    xorq \result, \result
    .irp reg, \regs
        addq %\reg, \result
    .endr
.endm

# Conditional macros based on architecture/features
.macro OPTIMIZED_MEMCPY
    .ifdef USE_AVX
        # AVX optimized memcpy (just a placeholder)
        vmovdqa (%rsi), %ymm0
        vmovdqa %ymm0, (%rdi)
    .elifdef USE_SSE
        # SSE optimized memcpy
        movdqa (%rsi), %xmm0
        movdqa %xmm0, (%rdi)
    .else
        # Standard memcpy
        movsb
    .endif
.endm

# Generate square table from 1 to 10
GENERATE_SQUARE_TABLE 1, 10

# Calculate factorial of 5 at assembly time
FACTORIAL 5
.set FACT_5, FACT_RESULT

main:
    pushq %rbp
    movq %rsp, %rbp
    
    # Example of using the compile-time factorial calculation
    movq $FACT_5, %rsi
    movq $fmt_int, %rdi
    xorq %rax, %rax
    call printf
    
    # Access the square table
    leaq square_table(%rip), %rbx
    
    # Print first 5 square values
    xorq %r12, %r12
1:
    cmpq $5, %r12
    jge 2f
    
    movq (%rbx,%r12,4), %rsi
    movq $fmt_int, %rdi
    xorq %rax, %rax
    call printf
    
    incq %r12
    jmp 1b
2:
    
    # Example of using SUM_REGISTERS macro
    movq $1, %r8
    movq $2, %r9
    movq $3, %r10
    movq $4, %r11
    
    SUM_REGISTERS %rax, r8, r9, r10, r11
    
    movq %rax, %rsi
    movq $fmt_int, %rdi
    xorq %rax, %rax
    call printf
    
    xorq %rax, %rax
    leave
    ret

# Fibonacci sequence in data section
.section .data
fibonacci_sequence:
    FIBONACCI 10
    