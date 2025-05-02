# 17_structs.asm - Demonstrates struct-like data structures
# To assemble and link:
# as -o 17_structs.o 17_structs.s
# ld -o 17_structs 17_structs.o

# Define struct layout
.equ POINT_SIZE, 16         # Size of Point struct
.equ POINT_X, 0             # Offset of x field
.equ POINT_Y, 8             # Offset of y field

.equ RECT_SIZE, 32          # Size of Rectangle struct
.equ RECT_TOP_LEFT, 0       # Offset of top_left field (Point)
.equ RECT_BOTTOM_RIGHT, 16  # Offset of bottom_right field (Point)

.section .data
# Define a point struct
point1:
    .quad 10                # x coordinate
    .quad 20                # y coordinate

# Define a rectangle struct (consisting of two points)
rectangle:
    .quad 5                 # top_left.x
    .quad 5                 # top_left.y
    .quad 15                # bottom_right.x
    .quad 10                # bottom_right.y

.section .bss
    .lcomm new_point, POINT_SIZE  # Uninitialized point
    .lcomm new_rect, RECT_SIZE    # Uninitialized rectangle

.section .text
.global _start

# Function to create a point
# Arguments: %rdi = x, %rsi = y, %rdx = pointer to point struct
# Returns: nothing
create_point:
    movq %rdi, POINT_X(%rdx)  # Store x coordinate
    movq %rsi, POINT_Y(%rdx)  # Store y coordinate
    ret

# Function to calculate distance between two points
# Arguments: %rdi = pointer to point1, %rsi = pointer to point2
# Returns: %rax = square of distance
point_distance_squared:
    # Calculate dx = p2.x - p1.x
    movq POINT_X(%rsi), %rax  # p2.x
    subq POINT_X(%rdi), %rax  # p2.x - p1.x
    
    # Square dx
    imulq %rax                # dx * dx
    movq %rax, %rbx           # Save dx^2
    
    # Calculate dy = p2.y - p1.y
    movq POINT_Y(%rsi), %rax  # p2.y
    subq POINT_Y(%rdi), %rax  # p2.y - p1.y
    
    # Square dy
    imulq %rax                # dy * dy
    
    # Add squares
    addq %rbx, %rax           # dx^2 + dy^2
    ret

# Function to calculate rectangle area
# Argument: %rdi = pointer to rectangle
# Returns: %rax = area
rectangle_area:
    # Calculate width = bottom_right.x - top_left.x
    movq RECT_BOTTOM_RIGHT+POINT_X(%rdi), %rax  # bottom_right.x
    subq RECT_TOP_LEFT+POINT_X(%rdi), %rax      # bottom_right.x - top_left.x
    
    # Save width
    movq %rax, %rbx
    
    # Calculate height = bottom_right.y - top_left.y
    movq RECT_BOTTOM_RIGHT+POINT_Y(%rdi), %rax  # bottom_right.y
    subq RECT_TOP_LEFT+POINT_Y(%rdi), %rax      # bottom_right.y - top_left.y
    
    # Calculate area = width * height
    imulq %rbx                # width * height
    ret

_start:
    # Create a new point
    movq $25, %rdi           # x = 25
    movq $35, %rsi           # y = 35
    movq $new_point, %rdx    # pointer to new_point
    call create_point        # Create the point
    
    # Calculate distance between points
    movq $point1, %rdi       # pointer to point1
    movq $new_point, %rsi    # pointer to new_point
    call point_distance_squared # Calculate distance squared
    
    # Save result
    pushq %rax               # Save on stack
    
    # Calculate rectangle area
    movq $rectangle, %rdi    # pointer to rectangle
    call rectangle_area      # Calculate area
                             # %rax now contains 100 (10 * 10)
    
    # Exit with result as status
    movq %rax, %rdi          # Set exit status (100)
    andq $0xFF, %rdi         # Limit to 0-255 range
    movq $60, %rax           # syscall number for exit (60)
    syscall
    