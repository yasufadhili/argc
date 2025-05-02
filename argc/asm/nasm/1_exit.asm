section .text
global _start

_start:
  mov rax, 60 ; Call code for exit
  mov rdi, 0  ; Exit code
  syscall