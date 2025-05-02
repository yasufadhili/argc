.section .data
    filename: .asciz "example.txt"
    file_content: .asciz "This is written using assembly macros!\n"
    read_buffer: .space 100
    newline: .asciz "\n"
    readmsg: .asciz "Read from file: "
    error_msg: .asciz "Error: System call failed\n"

.section .text
    .global main

# System call constants for x86-64 Linux
.equ SYS_READ,   0
.equ SYS_WRITE,  1
.equ SYS_OPEN,   2
.equ SYS_CLOSE,  3
.equ SYS_EXIT,   60

# File open flags
.equ O_RDONLY,   0
.equ O_WRONLY,   1
.equ O_RDWR,     2
.equ O_CREAT,    64
.equ O_TRUNC,    512

# File permissions
.equ S_IRUSR,    400     # Read permission, owner
.equ S_IWUSR,    200     # Write permission, owner
.equ S_IRGRP,    40      # Read permission, group
.equ S_IWGRP,    20      # Write permission, group
.equ S_IROTH,    4       # Read permission, others
.equ S_IWOTH,    2       # Write permission, others

# Create a macro for system calls
.macro SYSCALL nr, arg1=0, arg2=0, arg3=0, arg4=0, arg5=0, arg6=0
    movq $\nr, %rax          # System call number
    movq \arg1, %rdi         # First argument
    movq \arg2, %rsi         # Second argument
    movq \arg3, %rdx         # Third argument
    movq \arg4, %r10         # Fourth argument
    movq \arg5, %r8          # Fifth argument 
    movq \arg6, %r9          # Sixth argument
    syscall
    
    # Check for errors (negative return value)
    cmpq $0, %rax
    jge 9\@
    
    # Print error message
    pushq %rax
    movq $1, %rdi            # STDOUT
    leaq error_msg(%rip), %rsi # Error message
    movq $25, %rdx           # Length
    movq $SYS_WRITE, %rax
    syscall
    popq %rax
    negq %rax                # Convert to positive errno
    
9\@:                         # Continue execution
.endm

# File handling macros
.macro OPEN filename, flags, mode=0
    SYSCALL SYS_OPEN, $\filename, $\flags, $\mode
.endm

.macro WRITE fd, buffer, count
    SYSCALL SYS_WRITE, \fd, $\buffer, \count
.endm

.macro READ fd, buffer, count
    SYSCALL SYS_READ, \fd, $\buffer, \count
.endm

.macro CLOSE fd
    SYSCALL SYS_CLOSE, \fd
.endm

# Print string macro (calculates length automatically)
.macro PRINT_STRING str
    # Calculate string length using the string scan instruction
    pushq %rdi
    pushq %rcx
    pushq %rax
    
    leaq \str(%rip), %rdi
    movq $-1, %rcx
    xorb %al, %al
    repne scasb
    notq %rcx
    decq %rcx
    
    # Write to STDOUT
    WRITE $1, \str, %rcx
    
    popq %rax
    popq %rcx
    popq %rdi
.endm

main:
    pushq %rbp
    movq %rsp, %rbp
    
    # Create and write to file using macros
    OPEN filename, $(O_CREAT | O_WRONLY | O_TRUNC), $(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
    movq %rax, %r12          # Save file descriptor
    
    # Calculate length of file_content
    leaq file_content(%rip), %rdi
    movq $-1, %rcx
    xorb %al, %al
    repne scasb
    notq %rcx
    decq %rcx
    movq %rcx, %r13          # Save length
    
    # Write to file
    WRITE %r12, file_content, %r13
    CLOSE %r12
    
    # Open file for reading
    OPEN filename, $O_RDONLY
    movq %rax, %r12
    
    # Read from file
    READ %r12, read_buffer, $99
    movq %rax, %r13          # Save bytes read
    
    # Null-terminate the buffer
    movb $0, read_buffer(%r13)
    
    # Print what we read
    PRINT_STRING readmsg
    PRINT_STRING read_buffer
    PRINT_STRING newline
    
    # Close file
    CLOSE %r12
    
    # Exit program
    SYSCALL SYS_EXIT, $0
    