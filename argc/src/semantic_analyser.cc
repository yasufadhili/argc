#include <memory>

#include "include/ast.hh"
#include "sym_table.hh"
#include "util_logger.hh"

using namespace ast;


void SemanticAnalyser::visit(unit::TranslationUnit& tu) {
  for (auto& m : tu.modules()) {
    m->accept(*this);
  }
}

void SemanticAnalyser::visit(mod::Module&m) {
  symbol_table_->enter_scope(m.identifier()->name());
  
  // Process functions first to allow forward references
  for (auto& f : m.functions()) {
    f->accept(*this);
  }

  for (auto & s : m.statements()) {
    s->accept(*this);
  }

  symbol_table_->exit_scope();
}

void SemanticAnalyser::visit(func::Function&f) {
  symbol_table_->enter_scope(f.name()->name());
  std::shared_ptr<sym_table::Type> func_type = std::make_shared<sym_table::Type>(
    sym_table::Type::TypeKind::FUNCTION,
    f.name()->name()
  );

  f.body()->accept(*this);
  
  symbol_table_->exit_scope();
}

void SemanticAnalyser::visit(func::Parameter&) {

}

void SemanticAnalyser::visit(func::ReturnTypeInfo&) {

}

void SemanticAnalyser::visit(func::MultipleReturnType&) {

}

void SemanticAnalyser::visit(func::SingleReturnType&) {

}

void SemanticAnalyser::visit(func::Body&) {

}

void SemanticAnalyser::visit(ident::Identifier&) {

}

void SemanticAnalyser::visit(ident::TypeIdentifier&) {

}

void SemanticAnalyser::visit(stmt::Statement&) {

}

void SemanticAnalyser::visit(stmt::Block&) {

}

void SemanticAnalyser::visit(stmt::Empty&) {

}

void SemanticAnalyser::visit(stmt::Return&) {

}

void SemanticAnalyser::visit(stmt::Print&) {

}

void SemanticAnalyser::visit(stmt::VariableDeclaration&) {

}

void SemanticAnalyser::visit(stmt::Assignment&) {

}

void SemanticAnalyser::visit(expr::Expression&) {

}

void SemanticAnalyser::visit(expr::Literal&) {

}

void SemanticAnalyser::visit(expr::Binary&) {

}

void SemanticAnalyser::visit(expr::Unary&) {

}

void SemanticAnalyser::visit(expr::Variable&) {

}