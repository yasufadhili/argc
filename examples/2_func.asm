  .section .text
  .global _start
_start:
  call main
  mov $60, %rax
  xor %rdi, %rdi
  syscall
some_return:
  pushq %rbp
  movq %rsp, %rbp
  movq %rbp, %rsp
  popq %rbp
  ret
main:
  pushq %rbp
  movq %rsp, %rbp
  movq %rbp, %rsp
  popq %rbp
  ret
