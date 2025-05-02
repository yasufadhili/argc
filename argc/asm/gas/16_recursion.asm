# 16_recursion.asm - Demonstrates recursive functions
# To assemble and link:
# as -o 16_recursion.o 16_recursion.s
# ld -o 16_recursion 16_recursion.o

.section .text
.global _start

# Recursive function to calculate factorial
# Argument: %rdi = n
# Returns: %rax = n!
factorial:
    # Base case
    cmpq $1, %rdi           # Compare n with 1
    jle factorial_base      # If n <= 1, handle base case
    
    # Save n and return address
    pushq %rdi              # Save n
    
    # Recursive call
    decq %rdi               # n-1
    call factorial          # Calculate (n-1)!
    
    # Multiply result by n
    popq %rdi               # Restore n
    imulq %rdi, %rax        # n * (n-1)!
    ret
    
factorial_base:
    movq $1, %rax           # Base case: 1! = 1, 0! = 1
    ret

# Recursive function to calculate Fibonacci number
# Argument: %rdi = n
# Returns: %rax = fib(n)
fibonacci:
    # Base cases
    cmpq $0, %rdi           # Compare n with 0
    je fib_zero             # If n == 0, return 0
    cmpq $1, %rdi           # Compare n with 1
    je fib_one              # If n == 1, return 1
    
    # Save n and return address
    pushq %rdi              # Save n
    
    # Calculate fib(n-1)
    decq %rdi               # n-1
    call fibonacci          # Calculate fib(n-1)
    
    # Save fib(n-1)
    pushq %rax              # Save fib(n-1)
    
    # Calculate fib(n-2)
    popq %rbx               # Restore fib(n-1) to %rbx
    popq %rdi               # Restore n
    subq $2, %rdi           # n-2
    pushq %rbx              # Save fib(n-1) again
    
    call fibonacci          # Calculate fib(n-2)
    
    # Add fib(n-1) + fib(n-2)
    popq %rbx               # Restore fib(n-1)
    addq %rbx, %rax         # fib(n-1) + fib(n-2)
    ret
    
fib_zero:
    movq $0, %rax           # fib(0) = 0
    ret
    
fib_one:
    movq $1, %rax           # fib(1) = 1
    ret

# Recursive function to calculate GCD using Euclidean algorithm
# Arguments: %rdi = a, %rsi = b
# Returns: %rax = gcd(a, b)
gcd:
    # Check if b == 0
    cmpq $0, %rsi           # Compare b with 0
    je gcd_base             # If b == 0, return a
    
    # Calculate a % b
    movq %rdi, %rax         # a
    movq $0, %rdx           # Clear %rdx for division
    divq %rsi               # a / b, remainder in %rdx
    
    # Recursive call: gcd(b, a % b)
    movq %rsi, %rdi         # New a = old b
    movq %rdx, %rsi         # New b = a % b
    call gcd                # Calculate gcd(b, a % b)
    ret
    
gcd_base:
    movq %rdi, %rax         # gcd(a, 0) = a
    ret

_start:
    # Calculate factorial(5)
    movq $5, %rdi           # n = 5
    call factorial          # Calculate 5!
                            # %rax now contains 120
    
    # Save result
    pushq %rax              # Save on stack
    
    # Calculate fibonacci(7)
    movq $7, %rdi           # n = 7
    call fibonacci          # Calculate fib(7)
                            # %rax now contains 13
    
    # Save result
    pushq %rax              # Save on stack
    
    # Calculate gcd(48, 18)
    movq $48, %rdi          # a = 48
    movq $18, %rsi          # b = 18
    call gcd                # Calculate gcd(48, 18)
                            # %rax now contains 6
    
    # Exit with result as status
    movq %rax, %rdi         # Set exit status (6)
    movq $60, %rax          # syscall number for exit (60)
    syscall
    