#include "../include/ast.hh"


using namespace ast::param;

void Parameter::print(const int level) {
  print_indent(level);
  std::cout << "Parameter \n";
}

