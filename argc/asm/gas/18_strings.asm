# 18_strings.asm - Demonstrates string manipulation
# To assemble and link:
# as -o 18_strings.o 18_strings.s
# ld -o 18_strings 18_strings.o

.section .data
hello_str:
    .ascii "Hello, World!\0"

greeting:
    .ascii "Greetings\0"

buffer:
    .space 64, 0             # 64-byte buffer filled with zeros

newline:
    .ascii "\n"

.section .text
.global _start

# Function to calculate string length (like strlen)
# Argument: %rdi = pointer to null-terminated string
# Returns: %rax = length of string
strlen:
    movq %rdi, %rax         # Copy string pointer
    
strlen_loop:
    cmpb $0, (%rax)         # Check if current byte is null
    je strlen_done          # If null, we're done
    incq %rax               # Move to next byte
    jmp strlen_loop         # Repeat
    
strlen_done:
    subq %rdi, %rax         # Calculate length (end - start)
    ret

# Function to copy strings (like strcpy)
# Arguments: %rdi = destination, %rsi = source
# Returns: %rax = pointer to destination
strcpy:
    movq %rdi, %rax         # Return value is destination
    
strcpy_loop:
    movb (%rsi), %bl        # Load byte from source
    movb %bl, (%rdi)        # Store byte to destination
    
    cmpb $0, %bl            # Check if byte is null
    je strcpy_done          # If null, we're done
    
    incq %rdi               # Move to next byte in destination
    incq %rsi               # Move to next byte in source
    jmp strcpy_loop         # Repeat
    
strcpy_done:
    ret

# Function to concatenate strings (like strcat)
# Arguments: %rdi = destination, %rsi = source
# Returns: %rax = pointer to destination
strcat:
    pushq %rdi              # Save destination pointer
    
    # Find end of destination string
strcat_find_end:
    cmpb $0, (%rdi)         # Check if current byte is null
    je strcat_copy          # If null, start copying
    incq %rdi               # Move to next byte
    jmp strcat_find_end     # Repeat
    
strcat_copy:
    # Copy source string to end of destination
    movb (%rsi), %bl        # Load byte from source
    movb %bl, (%rdi)        # Store byte to destination
    
    cmpb $0, %bl            # Check if byte is null
    je strcat_done          # If null, we're done
    
    incq %rdi               # Move to next byte in destination
    incq %rsi               # Move to next byte in source
    jmp strcat_copy         # Repeat
    
strcat_done:
    popq %rax               # Restore and return original destination pointer
    ret

_start:
    # Calculate length of hello_str
    movq $hello_str, %rdi   # pointer to string
    call strlen             # Calculate length
                            # %rax now contains 13
    
    # Save result
    pushq %rax              # Save on stack
    
    # Copy hello_str to buffer
    movq $buffer, %rdi      # destination
    movq $hello_str, %rsi   # source
    call strcpy             # Copy string
    
    # Print the copied string
    movq $1, %rax           # syscall number for write (1)
    movq $1, %rdi           # file descriptor 1 (stdout)
    movq $buffer, %rsi      # pointer to string
    movq $13, %rdx          # length of string
    syscall
    
    # Print a newline
    movq $1, %rax           # syscall number for write (1)
    movq $1, %rdi           # file descriptor 1 (stdout)
    movq $newline, %rsi     # pointer to newline
    movq $1, %rdx           # length of newline
    syscall
    
    # Concatenate greeting with buffer
    movq $buffer, %rdi      # destination
    movq $greeting, %rsi    # source
    call strcat             # Concatenate
    
    # Calculate new length
    movq $buffer, %rdi      # pointer to string
    call strlen             # Calculate length
                            # %rax now contains length of concatenated string
    
    # Print the concatenated string
    movq %rax, %rdx         # length of string
    movq $1, %rax           # syscall number for write (1)
    movq $1, %rdi           # file descriptor 1 (stdout)
    movq $buffer, %rsi      # pointer to string
    syscall
    
    # Exit with success
    movq $0, %rdi           # exit status 0
    movq $60, %rax          # syscall number for exit (60)
    syscall
    