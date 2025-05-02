# 04_arithmetic.asm - Demonstrates multiplication, division, increment, decrement
# To assemble and link:
# as -o 04_arithmetic.o 04_arithmetic.s
# ld -o 04_arithmetic 04_arithmetic.o

.section .text
.global _start

_start:
    # Multiplication
    movq $5, %rax       # rax = 5
    movq $10, %rbx      # rbx = 10
    imulq %rbx          # rax = rax * rbx (50)
                        # Note: imul uses implicit operands
                        # rdx:rax = rax * rbx (for large results)
    
    # Division
    movq $100, %rax     # rax = 100
    movq $0, %rdx       # Clear rdx (remainder will go here)
    movq $3, %rbx       # rbx = 3
    idivq %rbx          # rax = rdx:rax / rbx (33)
                        # rdx = rdx:rax % rbx (1)
    
    # Increment and decrement
    incq %rax           # rax++ (34)
    decq %rax           # rax-- (33)
    
    # Convert result to exit code (limited to 0-255)
    andq $0xFF, %rax    # Extract lowest byte (33)
    
    # Syscall: exit(result)
    movq %rax, %rdi     # Set exit code to our result
    movq $60, %rax      # syscall number for exit (60)
    syscall             # invoke the syscall