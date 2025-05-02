# 14_stack.asm - Demonstrates stack operations and stack frames
# To assemble and link:
# as -o 14_stack.o 14_stack.s
# ld -o 14_stack 14_stack.o

.section .text
.global _start

# Function with proper stack frame and local variables
# Calculates the sum of squares for numbers 1 to n
# Argument: %rdi = n
# Returns: %rax = sum of squares
sum_of_squares:
    # Prologue - set up stack frame
    pushq %rbp              # Save old base pointer
    movq %rsp, %rbp         # Set new base pointer
    
    # Allocate space for local variables
    subq $16, %rsp          # Allocate 16 bytes
                            # [rbp-8]: loop counter
                            # [rbp-16]: running sum
    
    # Initialize local variables
    movq $1, -8(%rbp)       # Initialize counter to 1
    movq $0, -16(%rbp)      # Initialize sum to 0
    
loop_start:
    # Check loop condition
    movq -8(%rbp), %rax     # Load counter
    cmpq %rdi, %rax         # Compare with n
    jg loop_end             # Exit if counter > n
    
    # Calculate square of counter
    movq %rax, %rbx         # Copy counter to %rbx
    imulq %rbx              # Square it (%rax = %rax * %rbx)
    
    # Add to sum
    addq %rax, -16(%rbp)    # Add square to sum
    
    # Increment counter
    incq -8(%rbp)           # counter++
    
    # Repeat loop
    jmp loop_start
    
loop_end:
    # Get result
    movq -16(%rbp), %rax    # Load sum into %rax for return
    
    # Epilogue - clean up stack frame
    movq %rbp, %rsp         # Restore stack pointer
    popq %rbp               # Restore base pointer
    ret                     # Return to caller

# Function that demonstrates push/pop for register preservation
# Calculates factorial using iterative approach
# Argument: %rdi = n
# Returns: %rax = n!
factorial_iter:
    # Save callee-saved registers we'll use
    pushq %rbx              # Save %rbx
    pushq %r12              # Save %r12
    
    # Initialize
    movq %rdi, %r12         # Store n in %r12
    movq $1, %rax           # Initialize result to 1
    movq $1, %rbx           # Initialize counter to 1
    
fact_loop:
    cmpq %r12, %rbx         # Compare counter with n
    jg fact_done            # Exit if counter > n
    
    imulq %rbx              # Multiply result by counter
    incq %rbx               # Increment counter
    jmp fact_loop           # Repeat
    
fact_done:
    # Restore callee-saved registers
    popq %r12               # Restore %r12
    popq %rbx               # Restore %rbx
    ret                     # Return to caller

_start:
    # Call sum_of_squares(5)
    movq $5, %rdi           # Argument n=5
    call sum_of_squares     # Call function
                            # %rax now has sum of squares (55)
    
    # Save result
    pushq %rax              # Save on stack
    
    # Call factorial_iter(6)
    movq $6, %rdi           # Argument n=6
    call factorial_iter     # Call function
                            # %rax now has 6! (720)
    
    # Exit with result modulo 256
    andq $0xFF, %rax        # Limit to 0-255 range
    movq %rax, %rdi         # Set exit status
    movq $60, %rax          # syscall number for exit (60)
    syscall
    