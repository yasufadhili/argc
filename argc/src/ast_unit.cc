#include <iostream>
#include <memory>
#include "../include/ast.hh"

using namespace ast::unit;

TranslationUnit::TranslationUnit(std::shared_ptr<stmt::Asm> a) {

}

void TranslationUnit::accept(SemanticAnalyser &) {

}

void TranslationUnit::accept(x86_64_CodeGenerator &) {

}


void TranslationUnit::print(const int level) {
  print_indent(level);
  std::cout << "Translation Unit:\n";
}
