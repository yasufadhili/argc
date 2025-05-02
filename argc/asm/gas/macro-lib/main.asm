# main.asm - Example program using our macro library

# Include our macro library
.include "macros.asm"

.section .data
    hello_msg: .asciz "Hello, Macro World!\n"
    file_name: .asciz "test.txt"
    write_data: .asciz "This is a test of our macro library.\n"
    read_buffer: .space 100
    read_msg: .asciz "Read from file: "
    newline: .asciz "\n"
    
.section .text
    .global main

# Example function using our macros
count_chars:
    FUNC_PROLOG
    
    # Function parameters:
    # - RDI: string pointer
    # - RSI: character to count
    xorq %rax, %rax          # Initialize counter to 0
    
1:
    movb (%rdi), %bl         # Load character
    testb %bl, %bl           # Check for null terminator
    jz 2f                    # Exit if found
    
    cmpb %sil, %bl           # Compare with target character
    jne 3f                   # Skip if not matching
    incq %rax                # Increment counter
    
3:
    incq %rdi                # Move to next character
    jmp 1b                   # Loop back
    
2:
    FUNC_EPILOG

main:
    FUNC_PROLOG 16           # Reserve 16 bytes on stack
    
    # Print hello message
    PRINT hello_msg
    
    # Create and write to a file
    OPEN file_name, $(O_CREAT | O_WRONLY | O_TRUNC), $(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
    movq %rax, -8(%rbp)      # Save file descriptor on stack
    
    # Get length of write_data
    STRLEN write_data, %r12
    
    # Write to file
    WRITE -8(%rbp), write_data, %r12
    
    # Close file
    CLOSE -8(%rbp)
    
    # Open file for reading
    OPEN file_name, $O_RDONLY
    movq %rax, -8(%rbp)
    
    # Read from file
    READ -8(%rbp), read_buffer, $99
    movq %rax, -16(%rbp)     # Save bytes read
    
    # Null-terminate buffer
    movb $0, read_buffer(%rax)
    
    # Print what we read
    PRINT read_msg
    PRINT read_buffer
    
    # Count occurrences of the letter 'a' in the buffer
    leaq read_buffer(%rip), %rdi
    movb $'a', %sil
    call count_chars
    
    # Convert result to string and print it (simplified for brevity)
    addq $'0', %rax          # Convert to ASCII (works only for single digits)
    movb %al, -16(%rbp)
    movb $'\n', -15(%rbp)
    WRITE $1, %rbp, $-15, $2
    
    # Close file
    CLOSE -8(%rbp)
    
    # Exit program
    EXIT $0
