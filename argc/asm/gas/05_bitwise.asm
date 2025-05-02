# 05_bitwise.asm - Demonstrates bitwise operations: and, or, xor, not, shl, shr
# to assemble and link:
# as -o 05_bitwise.o 05_bitwise.s
# ld -o 05_bitwise 05_bitwise.o

.section .text
.global _start

_start:
    # Bitwise AND
    movq $0b1010, %rax  # rax = 1010 binary (10 decimal)
    movq $0b1100, %rbx  # rbx = 1100 binary (12 decimal)
    andq %rbx, %rax     # rax = rax & rbx = 1000 binary (8 decimal)
    
    # Bitwise OR
    movq $0b1010, %rax  # rax = 1010 binary (10 decimal)
    movq $0b1100, %rbx  # rbx = 1100 binary (12 decimal)
    orq %rbx, %rax      # rax = rax | rbx = 1110 binary (14 decimal)
    
    # Bitwise XOR
    movq $0b1010, %rax  # rax = 1010 binary (10 decimal)
    movq $0b1100, %rbx  # rbx = 1100 binary (12 decimal)
    xorq %rbx, %rax     # rax = rax ^ rbx = 0110 binary (6 decimal)
    
    # Bitwise NOT
    movq $0b1111, %rax  # rax = 1111 binary (15 decimal)
    notq %rax           # rax = ~rax (all bits flipped)
                        # Result will have upper bits set too,
                        # so we'll clear them next
    andq $0xF, %rax     # Keep only lowest 4 bits = 0000 binary (0 decimal)
    
    # Shift left (multiply by 2 for each shift)
    movq $5, %rax       # rax = 5
    shlq $1, %rax       # rax = rax << 1 = 10
    shlq $2, %rax       # rax = rax << 2 = 40
    
    # Shift right (divide by 2 for each shift)
    movq $40, %rax      # rax = 40
    shrq $3, %rax       # rax = rax >> 3 = 5
    
    # Bit test (for checking specific bits)
    movq $0b10101, %rax # rax = 10101 binary (21 decimal)
    testq $0b00100, %rax # Test if bit 2 is set (it is)
                        # This performs AND but only sets flags
                        # ZF will be 0 since result is non-zero
    
    # Syscall: exit(5)
    movq $5, %rdi       # exit code 5
    movq $60, %rax      # syscall number for exit (60)
    syscall             # invoke the syscall