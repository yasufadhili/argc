#include "Visitor.hxx"

#include "AST.hxx"


auto ast::Visitor::visit(const prog::Program &prog) -> void {
  prog.units.at(0)->accept(*this);
}

auto ast::Visitor::visit(const unit::Unit &unit) -> void {
  unit.functions.at(0)->accept(*this);
}

auto ast::Visitor::visit(const func::Func &func) -> void {
  func.stmts.at(0)->accept(*this);
}

auto ast::Visitor::visit(const stmt::Return &ret_stmt) -> void {
  if (ret_stmt.expr.has_value()) {
    ret_stmt.expr.value()->accept(*this);
  } else {
    emit("    RET");
  }
}

auto ast::Visitor::visit(const expr::IntLit &int_lit) -> void {
  emit("    MOV eax, " + std::to_string(int_lit.value));
}




