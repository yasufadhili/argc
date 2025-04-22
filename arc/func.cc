#include "include/ast.hh"


using namespace ast::func;

Function::Function() = default;

Function::~Function() = default;

void Function::print(const int level) {
  print_indent(level);
}



