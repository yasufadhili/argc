# 19_switch_case.asm - Demonstrates switch/case using jump tables
# To assemble and link:
# as -o 19_switch_case.o 19_switch_case.s
# ld -o 19_switch_case 19_switch_case.o

.section .data
# Jump table for handling different cases
jump_table:
    .quad case_0            # Case 0 handler
    .quad case_1            # Case 1 handler
    .quad case_2            # Case 2 handler
    .quad case_3            # Case 3 handler
    .quad default_case      # Default case handler

# Messages for each case
msg_0:
    .ascii "Case 0: Adding values\n"
msg_0_end:

msg_1:
    .ascii "Case 1: Subtracting values\n"
msg_1_end:

msg_2:
    .ascii "Case 2: Multiplying values\n"
msg_2_end:

msg_3:
    .ascii "Case 3: Dividing values\n"
msg_3_end:

msg_default:
    .ascii "Default case: Invalid operation\n"
msg_default_end:

.section .text
.global _start

# Function to handle a switch statement
# Arguments: %rdi = case value, %rsi = first operand, %rdx = second operand
# Returns: %rax = result of operation
switch_example:
    # Check if case is within bounds
    cmpq $0, %rdi           # Compare with lower bound
    jl switch_default       # If < 0, use default
    
    cmpq $3, %rdi           # Compare with upper bound
    jg switch_default       # If > 3, use default
    
    # Calculate jump address from table
    movq $jump_table, %rax  # Get address of jump table
    movq (%rax,%rdi,8), %rax # Get address for this case
    jmp *%rax               # Jump to case handler
    
case_0:
    # Addition
    movq $msg_0, %rsi       # pointer to message
    movq $msg_0_end - msg_0, %rdx # length of message
    call print_message      # Print message
    
    movq %rsi, %rax         # first operand
    addq %rdx, %rax         # Add second operand
    ret
    
case_1:
    # Subtraction
    movq $msg_1, %rsi       # pointer to message
    movq $msg_1_end - msg_1, %rdx # length of message
    call print_message      # Print message
    
    movq %rsi, %rax         # first operand
    subq %rdx, %rax         # Subtract second operand
    ret
    
case_2:
    # Multiplication
    movq $msg_2, %rsi       # pointer to message
    movq $msg_2_end - msg_2, %rdx # length of message
    call print_message      # Print message
    
    movq %rsi, %rax         # first operand
    imulq %rdx              # Multiply by second operand
    ret
    
case_3:
    # Division
    movq $msg_3, %rsi       # pointer to message
    movq $msg_3_end - msg_3, %rdx # length of message
    call print_message      # Print message
    
    movq %rsi, %rax         # first operand
    movq $0, %rdx           # Clear for division
    divq %rcx               # Divide by second operand
    ret
    
switch_default:
default_case:
    # Default case
    movq $msg_default, %rsi # pointer to message
    movq $msg_default_end - msg_default, %rdx # length of message
    call print_message      # Print message
    
    movq $-1, %rax          # Return -1 for error
    ret

# Helper function to print a message
# Arguments: %rsi = pointer to message, %rdx = length
print_message:
    pushq %rax              # Save registers
    pushq %rdi
    
    movq $1, %rax          # sys_write
    movq $1, %rdi          # stdout
    syscall
    
    popq %rdi              # Restore registers
    popq %rax
    ret

_start:
    # Test case 0 (Addition: 10 + 5)
    movq $0, %rdi          # Case 0
    movq $10, %rsi         # First operand
    movq $5, %rdx          # Second operand
    call switch_example

    # Test case 1 (Subtraction: 20 - 8)
    movq $1, %rdi          # Case 1
    movq $20, %rsi         # First operand
    movq $8, %rdx          # Second operand
    call switch_example

    # Test case 2 (Multiplication: 6 * 7)
    movq $2, %rdi          # Case 2
    movq $6, %rsi          # First operand
    movq $7, %rdx          # Second operand
    call switch_example

    # Test case 3 (Division: 100 / 5)
    movq $3, %rdi          # Case 3
    movq $100, %rsi        # First operand
    movq $5, %rcx          # Second operand (note: using %rcx for division)
    call switch_example

    # Test invalid case (-1)
    movq $-1, %rdi         # Invalid case
    movq $0, %rsi          # First operand
    movq $0, %rdx          # Second operand
    call switch_example

    # Exit program
    movq $60, %rax         # sys_exit
    movq $0, %rdi          # status: 0
    syscall