.section .data
    message: .asciz "Hello, %s!\n"

.section .text
    .global main

# Define a print_hello macro that takes a name parameter
.macro print_hello name
    # Save registers
    pushq %rdi
    pushq %rsi
    pushq %rax
    
    # Call printf with "Hello, name!"
    movq $message, %rdi     # First argument: format string
    movq $\name, %rsi       # Second argument: name parameter
    xorq %rax, %rax         # Clear RAX (no vector registers used)
    call printf
    
    # Restore registers
    popq %rax
    popq %rsi
    popq %rdi
.endm

main:
    # Function prologue
    pushq %rbp
    movq %rsp, %rbp
    
    # Call our macro with different arguments
    print_hello world
    print_hello friend
    
    # Function epilogue and return
    movq $0, %rax
    leave
    ret

.section .data
    world: .asciz "World"
    friend: .asciz "Friend"