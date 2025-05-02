
#include "../include/ast.hh"

using namespace ast;

void SymbolCollector::visit(std::shared_ptr<unit::TranslationUnit> &tu) {
  for (auto& m : tu->modules()) {
    m->accept(*this);
  }
}

void SymbolCollector::visit(std::shared_ptr<ident::Identifier> &) {

}

void SymbolCollector::visit(std::shared_ptr<ident::TypeIdentifier> &) {

}


void SymbolCollector::visit(std::shared_ptr<mod::Module> &) {

}

void SymbolCollector::visit(std::shared_ptr<func::Function> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Statement> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Block> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Empty> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Assignment> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Return> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::VariableDeclaration> &) {

}

void SymbolCollector::visit(std::shared_ptr<stmt::Print> &) {

}

void SymbolCollector::visit(std::shared_ptr<expr::Expression> &) {

}

void SymbolCollector::visit(std::shared_ptr<expr::Literal> &) {

}

void SymbolCollector::visit(std::shared_ptr<expr::Binary> &) {

}

void SymbolCollector::visit(std::shared_ptr<expr::Unary> &) {

}

void SymbolCollector::visit(std::shared_ptr<expr::Variable> &) {

}

