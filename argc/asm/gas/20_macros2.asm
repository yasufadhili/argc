.section .data
    format_int: .asciz "%d\n"
    format_str: .asciz "%s\n"

.section .text
    .global main

# Macro with local labels using \@ for uniqueness
.macro PRINT_INT value
    pushq %rdi
    pushq %rsi
    pushq %rax
    
    movq $format_int, %rdi
    movq \value, %rsi
    xorq %rax, %rax
    call printf
    
    popq %rax
    popq %rsi
    popq %rdi
.endm

# Macro for creating a loop structure
.macro FOR_LOOP from=0, to, register, body:vararg
    movq $\from, \register      # Initialize the counter
    
1\@:                            # Local label, uniquified with \@
    cmpq $\to, \register        # Compare counter with the limit
    jge 2\@                     # Jump if greater or equal
    
    \body                       # Execute the body (vararg)
    
    incq \register              # Increment counter
    jmp 1\@                     # Jump back to start
2\@:                            # End of loop label
.endm

# Macro that accepts a variable number of arguments
.macro PRINT_STRINGS args:vararg
    .ifnb \args                 # If arguments are not blank
        pushq %rdi
        pushq %rsi
        pushq %rax
        
        # Get the first argument (using peeling)
        .ifnc "", "\\args"
            movq $format_str, %rdi
            movq $\args, %rsi
            xorq %rax, %rax
            call printf
        .endif
        
        popq %rax
        popq %rsi
        popq %rdi
    .endif
.endm

# Example of a nested macro
.macro PUSH_REGS regs:vararg
    .irp reg,\regs
        pushq %\reg
    .endr
.endm

.macro POP_REGS regs:vararg
    .irp reg,\regs
        popq %\reg
    .endr
.endm

.macro PRESERVE_REGS regs:vararg, body:vararg
    PUSH_REGS \regs
    \body
    POP_REGS \regs
.endm

main:
    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp              # Reserve stack space
    
    # Store some test values on the stack
    movq $10, -8(%rbp)
    movq $20, -16(%rbp)
    
    # Use the PRINT_INT macro
    PRINT_INT $42               # Immediate value
    PRINT_INT -8(%rbp)          # Value from memory
    
    # Use the FOR_LOOP macro with our PRINT_INT macro
    FOR_LOOP 1, 5, %r12, PRINT_INT $%r12
    
    # Use the string printing macro
    PRINT_STRINGS str1
    PRINT_STRINGS str2
    
    # Use the nested register preservation macro
    PRESERVE_REGS rax rbx rcx, movq $42, %rax
    
    movq $0, %rax
    leave
    ret

.section .data
    str1: .asciz "Hello from macro"
    str2: .asciz "Macros are powerful"