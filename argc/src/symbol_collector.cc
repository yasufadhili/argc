
#include "include/ast.hh"
#include "sym_table.hh"
#include <memory>

using namespace ast;

void SymbolCollector::visit(unit::TranslationUnit&tu) {
  for (auto& m : tu.modules()) {
    m->accept(*this);
  }
}

void SymbolCollector::visit(ident::Identifier&) {
}

void SymbolCollector::visit(ident::TypeIdentifier&) {

}


void SymbolCollector::visit(mod::Module&m) {
  for (auto f : m.functions()) {
    f->accept(*this);
  }
}

void SymbolCollector::visit(func::Function&) {

}

void SymbolCollector::visit(stmt::Statement&) {

}

void SymbolCollector::visit(stmt::Block&b) {
  for (auto & stmt : b.statements()) {
    stmt->accept(*this);
  }
}

void SymbolCollector::visit(stmt::Empty&) {

}

void SymbolCollector::visit(stmt::Assignment& s) {
  s.target()->accept(*this);
  s.value()->accept(*this);
}

void SymbolCollector::visit(stmt::Return& rs) {
  if (rs.expression()) {
    rs.expression().value()->accept(*this);
  }
}

void SymbolCollector::visit(stmt::VariableDeclaration& vd) {
  if (vd.initialiser()) {
    vd.initialiser().value()->accept(*this);
  }
}

void SymbolCollector::visit(stmt::Print& ps) {
  ps.expression()->accept(*this);
}

void SymbolCollector::visit(expr::Expression&) {

}

void SymbolCollector::visit(expr::Literal&) {

}

void SymbolCollector::visit(expr::Binary& e) {
  if (e.lhs()) {
    e.lhs()->accept(*this);
  }
  if (e.rhs()) {
    e.rhs()->accept(*this);
  }
}

void SymbolCollector::visit(expr::Unary& e) {
  e.operand()->accept(*this);
}

void SymbolCollector::visit(expr::Variable& e) {
  e.identifier()->accept(*this);
}

