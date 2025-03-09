
#include "AST.hxx"

namespace ast {


  Visitor::Visitor() {
    emit("section .data \n");
    emit("EXIT_SUCCESS equ 0");
    emit("SYS_exit equ 60 \n");
    emit("section .text \n");
    emit("global _start \n");
    emit("_start:");

    //emit("last:");
    //emit("    MOV rax, SYS_exit");
    //emit("    MOV rdi, EXIT_SUCCESS");
    //emit("    syscall");
  }

  auto Visitor::visit(const stmt::Exit &exit) -> void {
    emit("    MOV rax, SYS_exit"); // defined the section .data
    emit("    MOV rdi, " + exit.value);
    emit("    syscall");
  }



}


