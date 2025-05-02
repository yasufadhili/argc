# 08_loops.asm - Demonstrates different loop constructs
# To assemble and link:
# as -o 08_loops.o 08_loops.s
# ld -o 08_loops 08_loops.o

.section .data
newline:
    .ascii "\n"

.section .bss
    .lcomm counter, 8   # 8-byte space for a counter

.section .text
.global _start

_start:
    # While loop example (sum numbers 1 to 5)
    movq $0, %rax       # Initialize sum to 0
    movq $1, %rbx       # Initialize counter to 1
    
while_loop:
    cmpq $6, %rbx       # Compare counter with 6
    jge while_end       # Exit loop if counter >= 6
    
    addq %rbx, %rax     # Add counter to sum
    incq %rbx           # Increment counter
    
    jmp while_loop      # Repeat loop
    
while_end:
    # Now %rax contains sum (15)
    
    # For loop example (count down from 5 to 1)
    movq $5, %rcx       # Initialize counter to 5
    
for_loop:
    # Loop body
    movq %rcx, counter  # Store current counter value
    
    # Convert counter to ASCII for printing
    addq $48, counter   # '0' is 48 in ASCII
    
    # Print counter
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $counter, %rsi # pointer to counter
    movq $1, %rdx       # length of counter (1 byte)
    syscall
    
    # Print newline
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $newline, %rsi # pointer to newline
    movq $1, %rdx       # length of newline (1 byte)
    syscall
    
    # Decrement counter and check for loop end
    decq %rcx           # Decrement counter
    cmpq $0, %rcx       # Compare with 0
    jg for_loop         # If counter > 0, continue loop
    
    # Do-while loop example (print numbers 1 to 3)
    movq $1, %rcx       # Initialize counter to 1
    
do_while_loop:
    # Loop body
    movq %rcx, counter  # Store current counter value
    
    # Convert counter to ASCII for printing
    addq $48, counter   # '0' is 48 in ASCII
    
    # Print counter
    movq $1, %rax       # syscall number for write (1)
    movq $1, %rdi       # file descriptor 1 (stdout)
    movq $counter, %rsi # pointer to counter
    movq $1, %rdx       # length of counter (1 byte)
    syscall
    
    # Increment counter
    incq %rcx
    
    # Loop condition (do-while always executes at least once)
    cmpq $4, %rcx       # Compare with 4
    jl do_while_loop    # If counter < 4, continue loop
    
    # Exit program
    movq $60, %rax      # syscall number for exit (60)
    movq $0, %rdi       # exit status 0
    syscall