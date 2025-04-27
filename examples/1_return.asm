  .section .text
  .global _start
  .type _start, @function
_start:
  mov $60, %rax
  xor %rdi, %rdi
  syscall
