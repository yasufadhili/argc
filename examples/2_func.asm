  .section .text
  .global _start
  .type _start, @function
printHello:
  pushq %rbp
  movq %rsp, %rbp
  movq %rbp, %rsp
  popq %rbp
  ret
_start:
  pushq %rbp
  movq %rsp, %rbp
  mov $60, %rax
  xor %rdi, %rdi
  syscall
  movq %rbp, %rsp
  popq %rbp
  ret
