# 06_comparisons.asm - Demonstrates comparison instructions and flags
# To assemble and link:
# as -o 06_comparisons.o 06_comparisons.s
# ld -o 06_comparisons 06_comparisons.o

.section .text
.global _start

_start:
    # Compare two numbers
    movq $10, %rax      # rax = 10
    movq $20, %rbx      # rbx = 20
    cmpq %rbx, %rax     # Compare rax with rbx
                        # This is equivalent to rax - rbx
                        # Sets flags based on the result
                        # In this case, rax < rbx, so:
                        # SF=1 (sign flag, result is negative)
                        # ZF=0 (zero flag, result is not zero)
                        # CF=1 (carry flag, borrowed in subtraction)
    
    # Test if a register is zero
    movq $0, %rcx       # rcx = 0
    testq %rcx, %rcx    # Tests if rcx is zero
                        # Performs rcx & rcx but only sets flags
                        # ZF=1 (zero flag, result is zero)
    
    # Test specific bits (useful for flags/masks)
    movq $0b10101, %rdx # rdx = 10101 binary (21 decimal)
    testq $0b00100, %rdx # Test if bit 2 is set
                        # ZF=0 (bit is set)
    
    # Compare for equality
    movq $25, %rax      # rax = 25
    movq $25, %rbx      # rbx = 25
    cmpq %rbx, %rax     # Compare rax and rbx
                        # ZF=1 (result of subtraction is zero)
                        # SF=0 (result is not negative)
    
    # Exit with a status based on our test
    movq $60, %rax      # syscall number for exit (60)
    movq $1, %rdi       # exit status 1
    syscall             # invoke the syscall