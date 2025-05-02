# 10_functions.asm - Demonstrates simple function calls
# To assemble and link:
# as -o 10_functions.o 10_functions.s
# ld -o 10_functions 10_functions.o

.section .text
.global _start

# Function that doubles a value
double:
    # The value to double is in %rdi (first argument register)
    addq %rdi, %rdi         # Double the value
    ret                     # Return (return value in %rax)

# Function that squares a value
square:
    # The value to square is in %rdi (first argument register)
    movq %rdi, %rax         # Copy input to %rax
    imulq %rdi, %rax        # %rax = %rdi * %rdi (square)
    ret                     # Return (return value in %rax)

_start:
    # Call double function with argument 5
    movq $5, %rdi           # Set up argument (5)
    call double             # Call the function
                            # Return value is now in %rax (10)
    
    # Save the result
    movq %rax, %rbx         # Save result in %rbx
    
    # Call square function with argument 4
    movq $4, %rdi           # Set up argument (4)
    call square             # Call the function
                            # Return value is now in %rax (16)
    
    # Add the two results
    addq %rbx, %rax         # %rax = %rax + %rbx (16 + 10 = 26)
    
    # Exit with the result as status
    andq $0xFF, %rax        # Limit to 0-255 range
    movq %rax, %rdi         # Set exit status
    movq $60, %rax          # syscall number for exit (60)
    syscall