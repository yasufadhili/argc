# 13_functions_multi_return.asm - Functions returning multiple values
# To assemble and link:
# as -o 13_functions_multi_return.o 13_functions_multi_return.s
# ld -o 13_functions_multi_return 13_functions_multi_return.o

.section .text
.global _start

# Function to divide two numbers, returning quotient and remainder
# Arguments: %rdi = dividend, %rsi = divisor
# Returns: %rax = quotient, %rdx = remainder
divide:
    movq %rdi, %rax         # Put dividend in %rax
    movq $0, %rdx           # Clear %rdx for division
    divq %rsi               # Unsigned divide, result in %rax, remainder in %rdx
    ret                     # Return values in %rax and %rdx

# Function to calculate min and max of two numbers
# Arguments: %rdi = first number, %rsi = second number
# Returns: %rax = min, %rdx = max
min_max:
    # Compare the two values
    cmpq %rsi, %rdi         # Compare first with second
    jle first_is_smaller    # Jump if first <= second
    
    # Second is smaller
    movq %rsi, %rax         # Min = second
    movq %rdi, %rdx         # Max = first
    ret
    
first_is_smaller:
    movq %rdi, %rax         # Min = first
    movq %rsi, %rdx         # Max = second
    ret

# Function to calculate sum and product of three numbers
# Arguments: %rdi = a, %rsi = b, %rdx = c
# Returns: %rax = sum, %rdx = product
sum_product:
    # Calculate sum (a + b + c)
    movq %rdi, %rax         # Start with a
    addq %rsi, %rax         # Add b
    addq %rdx, %rax         # Add c (sum is now in %rax)
    
    # Save sum temporarily
    pushq %rax              # Save sum on stack
    
    # Calculate product (a * b * c)
    movq %rdi, %rax         # Start with a
    imulq %rsi              # Multiply by b
    imulq %rdx              # Multiply by c (product is now in %rax)
    
    # Move product to %rdx
    movq %rax, %rdx         # Store product in %rdx
    
    # Restore sum
    popq %rax               # Restore sum to %rax
    
    ret                     # Return both values

_start:
    # Call divide(25, 7)
    movq $25, %rdi          # First argument (dividend)
    movq $7, %rsi           # Second argument (divisor)
    call divide             # Call function
                            # %rax now contains quotient (3)
                            # %rdx now contains remainder (4)
    
    # Save results
    pushq %rax              # Save quotient
    pushq %rdx              # Save remainder
    
    # Call min_max(10, 20)
    movq $10, %rdi          # First argument
    movq $20, %rsi          # Second argument
    call min_max            # Call function
                            # %rax now contains min (10)
                            # %rdx now contains max (20)
    
    # Save results
    pushq %rax              # Save min
    pushq %rdx              # Save max
    
    # Call sum_product(3, 4, 5)
    movq $3, %rdi           # First argument
    movq $4, %rsi           # Second argument
    movq $5, %rdx           # Third argument
    call sum_product        # Call function
                            # %rax now contains sum (12)
                            # %rdx now contains product (60)
    
    # Exit with product modulo 256 as status
    movq %rdx, %rdi         # Set exit status to product
    andq $0xFF, %rdi        # Limit to 0-255 range (60)
    movq $60, %rax          # syscall number for exit (60)
    syscall