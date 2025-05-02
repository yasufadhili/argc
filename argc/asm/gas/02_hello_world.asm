# 02_hello_world.asm - Print "Hello, World!" using Linux syscalls
# To assemble and link:
# as -o 02_hello_world.o 02_hello_world.s
# ld -o 02_hello_world 02_hello_world.o

.section .data          # Data section for initialized data
message:                # Label for our string
    .ascii "Hello, World!\n"  # String constant (without null terminator)
message_end:            # Label marking the end of the string

.section .text          # Text section contains code
.global _start          # _start is the entry point known by the linker

_start:
  # Syscall: write(1, message, len)
  movq $1, %rax       # syscall number for write (1)
  movq $1, %rdi       # file descriptor 1 (stdout)
  movq $message, %rsi # pointer to the message
  movq $message_end - message, %rdx # length of the message
  syscall             # invoke the syscall

  # Syscall: exit(0)
  movq $60, %rax      # syscall number for exit (60)
  movq $0, %rdi       # exit status 0 (success)
  syscall             # invoke the syscall