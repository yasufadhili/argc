# 11_functions_params.asm - Demonstrates functions with multiple parameters
# To assemble and link:
# as -o 11_functions_params.o 11_functions_params.s
# ld -o 11_functions_params 11_functions_params.o

.section .text
.global _start

# Function to add two numbers
# Arguments: %rdi = first number, %rsi = second number
# Returns: %rax = sum
add_numbers:
    movq %rdi, %rax         # Copy first argument to %rax
    addq %rsi, %rax         # Add second argument
    ret                     # Return (result in %rax)

# Function to multiply two numbers
# Arguments: %rdi = first number, %rsi = second number
# Returns: %rax = product
multiply_numbers:
    movq %rdi, %rax         # Copy first argument to %rax
    imulq %rsi, %rax        # Multiply by second argument
    ret                     # Return (result in %rax)

# Function that calculates (a*b) + c
# Arguments: %rdi = a, %rsi = b, %rdx = c
# Returns: %rax = (a*b) + c
calculate:
    # Save the third argument since we need to call another function
    pushq %rdx              # Save %rdx (c) on the stack
    
    # Call multiply_numbers(a, b)
    # Arguments are already in the right registers
    call multiply_numbers   # %rax now contains a*b
    
    # Retrieve c from stack
    popq %rdx               # Restore %rdx (c) from the stack
    
    # Add c to the product
    addq %rdx, %rax         # %rax = (a*b) + c
    
    ret                     # Return result in %rax

_start:
    # Call add_numbers(10, 20)
    movq $10, %rdi          # First argument
    movq $20, %rsi          # Second argument
    call add_numbers        # Call function
                            # %rax now contains 30
    
    # Save the result
    movq %rax, %rbx         # Save in %rbx
    
    # Call calculate(5, 6, 7) which computes (5*6) + 7
    movq $5, %rdi           # First argument (a)
    movq $6, %rsi           # Second argument (b)
    movq $7, %rdx           # Third argument (c)
    call calculate          # Call function
                            # %rax now contains (5*6) + 7 = 37
    
    # Add previous result
    addq %rbx, %rax         # %rax = 37 + 30 = 67
    
    # Exit with result as status
    movq %rax, %rdi         # Set exit status (67)
    andq $0xFF, %rdi        # Limit to 0-255 range
    movq $60, %rax          # syscall number for exit (60)
    syscall