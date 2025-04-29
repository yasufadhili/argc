#include <iostream>

#include "include/ast.hh"

using namespace ast::unit;

TranslationUnit::TranslationUnit()= default;

void TranslationUnit::accept(analyser::SemanticAnalyser &) {

}

void TranslationUnit::accept(codegen::x86_64_CodeGenerator &) {

}

void TranslationUnit::print(const int level) {
  print_indent(level);
  std::cout << "Translation Unit:\n";
}
