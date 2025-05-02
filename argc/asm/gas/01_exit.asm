# 01_exit.asm - Minimal program that exits with a status code
# To assemble and link: 
# as -o 01_exit.o 01_exit.s
# ld -o 01_exit 01_exit.o

.section .text      # Text section contains code
.global _start      # _start is the entry point known by the linker

_start:
  # Syscall: exit(0)
  movq $60, %rax  # syscall number for exit (60)
  movq $0, %rdi   # exit status 0 (success)
  syscall         # invoke the syscall