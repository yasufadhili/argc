#include <iostream>
#include <memory>
#include "../include/ast.hh"

using namespace ast::unit;

void TranslationUnit::accept(SemanticAnalyser &) {

}

void TranslationUnit::accept(CodeGenerator &) {
}


void TranslationUnit::print(const int level) {
  print_indent(level);
  std::cout << "Translation Unit\n";
}
