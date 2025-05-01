#include <iostream>
#include <memory>
#include "ast.hh"

using namespace ast::unit;

TranslationUnit::TranslationUnit(std::vector<std::shared_ptr<stmt::Statement> > stmts)
  : statements_(std::move(stmts))
{

}

void TranslationUnit::accept(SemanticAnalyser &an) {
  for (const auto& s : statements_) {
    an.visit(s);
  }
}

void TranslationUnit::accept(CodeGenerator &) {
}

void TranslationUnit::print(const int level) {
  print_indent(level);
  std::cout << "Translation Unit\n";
  for (const auto& stmt : statements_) {
    stmt->print(level + 1);
  }
}
