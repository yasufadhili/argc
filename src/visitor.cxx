#include "ast.hxx"


namespace ast {

  void Visitor::visit(prog::Program &prog) {
    emit("section .text \n");
    emit("global _start \n");
    for (const auto& func : prog.functions) {
      func->accept(*this);
    }
    emit("");
    emit("last:");
    emit("    MOV rax, 60");
    emit("    MOV rdi, 0");
    emit("    SYSCALL");
  }

  void Visitor::visit(const func::Function &func) {
    emit(func.name + ":");
    for (const auto&[fst, snd] : func.params) {
      //emit("Param: " + fst + " : " += snd);
    }
    if (func.name == "_start") {
      emit("    CALL last");
    }
  }

  void Visitor::visit(stmt::Statement &) {

  }

  void Visitor::visit(stmt::Block &) {

  }

  void Visitor::visit(stmt::Return &) {

  }

  void Visitor::visit(stmt::Exit &) {

  }







}

