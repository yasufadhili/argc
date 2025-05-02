# 15_scope_local_vars.asm - Demonstrates local variables and scope
# To assemble and link:
# as -o 15_scope_local_vars.o 15_scope_local_vars.s
# ld -o 15_scope_local_vars 15_scope_local_vars.o

.section .text
.global _start

# Function to calculate the distance between two points
# Arguments:
#   %rdi = x1, %rsi = y1 (first point)
#   %rdx = x2, %rcx = y2 (second point)
# Returns:
#   %rax = square of distance (to avoid floating point)
distance_squared:
    # Prologue
    pushq %rbp              # Save old base pointer
    movq %rsp, %rbp         # Set new base pointer
    
    # Allocate space for local variables
    subq $16, %rsp          # Allocate 16 bytes
                            # [rbp-8]: dx (x2-x1)
                            # [rbp-16]: dy (y2-y1)
    
    # Calculate dx = x2 - x1
    movq %rdx, %rax         # x2
    subq %rdi, %rax         # x2 - x1
    movq %rax, -8(%rbp)     # Store dx
    
    # Calculate dy = y2 - y1
    movq %rcx, %rax         # y2
    subq %rsi, %rax         # y2 - y1
    movq %rax, -16(%rbp)    # Store dy
    
    # Calculate dx^2
    movq -8(%rbp), %rax     # Load dx
    imulq %rax              # Square dx
    movq %rax, %rbx         # Save dx^2
    
    # Calculate dy^2
    movq -16(%rbp), %rax    # Load dy
    imulq %rax              # Square dy
    
    # Calculate dx^2 + dy^2
    addq %rbx, %rax         # %rax = dx^2 + dy^2
    
    # Epilogue
    movq %rbp, %rsp         # Restore stack pointer
    popq %rbp               # Restore base pointer
    ret                     # Return to caller

# Function to calculate average of an array of numbers
# Arguments:
#   %rdi = pointer to array
#   %rsi = length of array
# Returns:
#   %rax = average (truncated to integer)
average:
    # Prologue
    pushq %rbp              # Save old base pointer
    movq %rsp, %rbp         # Set new base pointer
    
    # Allocate space for local variables
    subq $16, %rsp          # Allocate 16 bytes
                            # [rbp-8]: sum
                            # [rbp-16]: counter
    
    # Initialize local variables
    movq $0, -8(%rbp)       # Initialize sum to 0
    movq $0, -16(%rbp)      # Initialize counter to 0
    
avg_loop:
    # Check if we're done
    movq -16(%rbp), %rax    # Load counter
    cmpq %rsi, %rax         # Compare with length
    jge avg_done            # Exit if counter >= length
    
    # Get array element
    movq %rax, %rbx         # Copy counter
    shlq $3, %rbx           # Multiply by 8 (size of quad)
    addq %rdi, %rbx         # Calculate address of element
    movq (%rbx), %rdx       # Load element
    
    # Add to sum
    addq %rdx, -8(%rbp)     # Add to sum
    
    # Increment counter
    incq -16(%rbp)          # counter++
    
    # Repeat loop
    jmp avg_loop
    
avg_done:
    # Calculate average
    movq -8(%rbp), %rax     # Load sum
    cmpq $0, %rsi           # Check if length is 0
    je avg_zero             # Handle division by zero
    
    movq $0, %rdx           # Clear upper bits for division
    divq %rsi               # Divide sum by length
    jmp avg_return
    
avg_zero:
    movq $0, %rax           # Return 0 if length is 0
    
avg_return:
    # Epilogue
    movq %rbp, %rsp         # Restore stack pointer
    popq %rbp               # Restore base pointer
    ret                     # Return to caller

# Test data
.section .data
points:
    .quad 1, 2              # First point (x1=1, y1=2)
    .quad 4, 6              # Second point (x2=4, y2=6)

numbers:
    .quad 10, 20, 30, 40, 50 # Test array
numbers_end:

_start:
    # Call distance_squared for the two points
    movq points, %rdi       # x1
    movq points+8, %rsi     # y1
    movq points+16, %rdx    # x2
    movq points+24, %rcx    # y2
    call distance_squared   # Call function
                            # %rax now contains distance squared (25)
    
    # Save result
    pushq %rax              # Save on stack
    
    # Calculate array length
    movq $numbers_end, %rax # End address
    subq $numbers, %rax     # Subtract start address
    shrq $3, %rax           # Divide by 8 (size of quad)
    movq %rax, %rsi         # Length in %rsi
    
    # Call average
    movq $numbers, %rdi     # Array pointer
    call average            # Call function
                            # %rax now contains average (30)
    
    # Exit with result as status
    movq %rax, %rdi         # Set exit status (30)
    movq $60, %rax          # syscall number for exit (60)
    syscall
    