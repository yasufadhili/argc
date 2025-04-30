#include "include/ast.hh"

using namespace ast::expr;

Literal::Literal(const int v)
  : value_(v){

}

void Literal::accept(SemanticAnalyser &) {

}

void Literal::accept(CodeGenerator &) {

}

void Literal::print(const int level) {
  print_indent(level);
  std::cout << "Literal \n";
}

