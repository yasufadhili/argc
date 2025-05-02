# 07_conditionals.asm - Demonstrates conditional jumps
# To assemble and link:
# as -o 07_conditionals.o 07_conditionals.s
# ld -o 07_conditionals 07_conditionals.o

.section .data
less_msg:
    .ascii "Number is less than 10\n"
less_msg_end:

equal_msg:
    .ascii "Number is equal to 10\n"
equal_msg_end:

greater_msg:
    .ascii "Number is greater than 10\n"
greater_msg_end:

.section .text
.global _start

_start:
    movq $10, %rbx      # Our comparison value
    movq $5, %rax       # The number to compare (change this to test different cases)
    
    # Compare rax with rbx
    cmpq %rbx, %rax     # Compare rax with rbx (rax - rbx)
    
    # Jump based on comparison result
    jl less_than        # Jump if less than (SF≠OF)
    je equal_to         # Jump if equal (ZF=1)
    jg greater_than     # Jump if greater than (ZF=0 and SF=OF)
    
less_than:
    # Print "less than" message
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $less_msg, %rsi # pointer to message
    movq $less_msg_end - less_msg, %rdx # length of message
    syscall
    jmp exit            # Jump to exit
    
equal_to:
    # Print "equal to" message
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $equal_msg, %rsi # pointer to message
    movq $equal_msg_end - equal_msg, %rdx # length of message
    syscall
    jmp exit            # Jump to exit
    
greater_than:
    # Print "greater than" message
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $greater_msg, %rsi # pointer to message
    movq $greater_msg_end - greater_msg, %rdx # length of message
    syscall
    # Fall through to exit
    
exit:
    # Exit program
    movq $60, %rax      # syscall number for exit (60)
    movq $0, %rdi       # exit status 0
    syscall