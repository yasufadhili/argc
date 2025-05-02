# 12_functions_return.asm - Functions with return values and proper stack frame
# To assemble and link:
# as -o 12_functions_return.o 12_functions_return.s
# ld -o 12_functions_return 12_functions_return.o

.section .text
.global _start

# Function to calculate factorial
# Argument: %rdi = number to calculate factorial for
# Returns: %rax = factorial result
factorial:
    # Set up stack frame
    pushq %rbp              # Save old base pointer
    movq %rsp, %rbp         # Set new base pointer
    
    # Check for base case (n <= 1)
    cmpq $1, %rdi           # Compare n with 1
    jle factorial_base_case # If n <= 1, handle base case
    
    # Recursive case: n! = n * (n-1)!
    pushq %rdi              # Save n
    
    decq %rdi               # n - 1
    call factorial          # Calculate (n-1)!
                            # Result in %rax
    
    popq %rdi               # Restore n
    imulq %rdi, %rax        # n * (n-1)!
    
    jmp factorial_end       # Skip base case
    
factorial_base_case:
    movq $1, %rax           # Base case: return 1
    
factorial_end:
    # Clean up stack frame
    movq %rbp, %rsp         # Restore stack pointer
    popq %rbp               # Restore base pointer
    ret                     # Return to caller

# Function to calculate fibonacci number
# Argument: %rdi = which fibonacci number to calculate
# Returns: %rax = fibonacci number
fibonacci:
    # Set up stack frame
    pushq %rbp              # Save old base pointer
    movq %rsp, %rbp         # Set new base pointer
    
    # Check base cases
    cmpq $0, %rdi           # Check if n == 0
    je fibonacci_zero
    cmpq $1, %rdi           # Check if n == 1
    je fibonacci_one
    
    # Recursive case: fib(n) = fib(n-1) + fib(n-2)
    # Calculate fib(n-1)
    pushq %rdi              # Save n
    
    decq %rdi               # n - 1
    call fibonacci          # Calculate fib(n-1)
                            # Result in %rax
    
    popq %rdi               # Restore n
    pushq %rax              # Save fib(n-1)
    pushq %rdi              # Save n again
    
    # Calculate fib(n-2)
    subq $2, %rdi           # n - 2
    call fibonacci          # Calculate fib(n-2)
                            # Result in %rax
    
    popq %rdi               # Restore n (not used anymore)
    popq %rbx               # Get fib(n-1)
    
    # Add results
    addq %rbx, %rax         # fib(n-1) + fib(n-2)
    
    jmp fibonacci_end       # Skip base cases
    
fibonacci_zero:
    movq $0, %rax           # Base case: fib(0) = 0
    jmp fibonacci_end
    
fibonacci_one:
    movq $1, %rax           # Base case: fib(1) = 1
    
fibonacci_end:
    # Clean up stack frame
    movq %rbp, %rsp         # Restore stack pointer
    popq %rbp               # Restore base pointer
    ret                     # Return to caller

_start:
    # Calculate factorial of 5
    movq $5, %rdi           # Argument for factorial
    call factorial          # Calculate 5!
                            # Result in %rax (120)
    
    # Save the result
    movq %rax, %rbx         # Save in %rbx
    
    # Calculate 7th fibonacci number (index 6)
    movq $6, %rdi           # Argument for fibonacci
    call fibonacci          # Calculate fibonacci(6)
                            # Result in %rax (8)
    
    # Exit with result as status
    movq %rax, %rdi         # Set exit status
    movq $60, %rax          # syscall number for exit (60)
    syscall