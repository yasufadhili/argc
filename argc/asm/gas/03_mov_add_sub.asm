# 03_mov_add_sub.asm - Demonstrates mov, add, and sub instructions
# To assemble and link:
# as -o 03_mov_add_sub.o 03_mov_add_sub.s
# ld -o 03_mov_add_sub 03_mov_add_sub.o

.section .data
result_msg:
    .ascii "Result: "
result_msg_end:

.section .bss           # Section for uninitialized data
    .lcomm result, 8    # Reserve 8 bytes for our result

.section .text
.global _start

_start:
    # Basic register operations
    movq $10, %rax      # rax = 10
    movq $15, %rbx      # rbx = 15
    
    # Addition
    addq %rbx, %rax     # rax = rax + rbx (25)
    
    # Subtraction
    movq $5, %rcx       # rcx = 5
    subq %rcx, %rax     # rax = rax - rcx (20)
    
    # Converting result to ASCII and storing
    addq $48, %rax      # Convert result to ASCII ('0' is 48 in ASCII)
    movq %rax, result   # Store in result
    
    # Syscall: write(1, result_msg, len)
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $result_msg, %rsi # pointer to the message
    movq $result_msg_end - result_msg, %rdx # length of the message
    syscall             # invoke the syscall
    
    # Syscall: write(1, result, 1)
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $result, %rsi  # pointer to the result
    movq $1, %rdx       # length of the result (1 byte)
    syscall             # invoke the syscall
    
    # Syscall: exit(0)
    movq $60, %rax      # syscall number for exit (60)
    movq $0, %rdi       # exit status 0 (success)
    syscall             # invoke the syscall