# 09_arrays.asm - Demonstrates array access and manipulation
# To assemble and link:
# as -o 09_arrays.o 09_arrays.s
# ld -o 09_arrays 09_arrays.o

.section .data
array:                  # Define an array of 5 long integers
    .quad 10, 20, 30, 40, 50
array_end:              # Label marking the end of the array

newline:
    .ascii "\n"

.section .bss
    .lcomm result, 8    # 8-byte space for a result value

.section .text
.global _start

_start:
    # Calculate array length
    movq $array_end, %rax   # Get address of array_end
    subq $array, %rax       # Subtract address of array start
    shrq $3, %rax           # Divide by 8 (size of quad) to get count
                            # Now %rax contains array length (5)
    
    # Initialize counter for loop
    movq $0, %rbx           # Index counter
    movq $0, %rcx           # Sum accumulator
    
array_loop:
    cmpq %rax, %rbx         # Compare index with array length
    jge array_loop_end      # If index >= length, exit loop
    
    # Load array element
    movq array(,%rbx,8), %rdx  # Load array[index] into rdx
                               # Base address + (index * element_size)
    
    # Add to sum
    addq %rdx, %rcx          # Add element to sum
    
    # Increment counter
    incq %rbx                # index++
    
    # Continue loop
    jmp array_loop
    
array_loop_end:
    # Now %rcx contains sum of array elements (150)
    
    # Modify an array element
    movq $2, %rbx           # Index 2 (third element)
    movq $100, array(,%rbx,8) # Set array[2] = 100
    
    # Access the modified element
    movq array(,%rbx,8), %rdx # Load array[2] into rdx (now 100)
    
    # Store result for exit code
    movq %rdx, %rdi         # Exit with the value we loaded (100)
    andq $0xFF, %rdi        # Limit to 0-255 range
    
    # Exit program
    movq $60, %rax          # syscall number for exit (60)
    syscall