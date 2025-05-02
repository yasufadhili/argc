
#include "include/ast.hh"

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


void SymbolCollector::visit(mod::Module&) {

}

void SymbolCollector::visit(func::Function&) {

}

void SymbolCollector::visit(stmt::Statement&) {

}

void SymbolCollector::visit(stmt::Block&) {

}

void SymbolCollector::visit(stmt::Empty&) {

}

void SymbolCollector::visit(stmt::Assignment&) {

}

void SymbolCollector::visit(stmt::Return&) {

}

void SymbolCollector::visit(stmt::VariableDeclaration&) {

}

void SymbolCollector::visit(stmt::Print&) {

}

void SymbolCollector::visit(expr::Expression&) {

}

void SymbolCollector::visit(expr::Literal&) {

}

void SymbolCollector::visit(expr::Binary&) {

}

void SymbolCollector::visit(expr::Unary&) {

}

void SymbolCollector::visit(expr::Variable&) {

}

