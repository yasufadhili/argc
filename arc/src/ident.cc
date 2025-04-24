#include "../include/ast.hh"


using namespace ast::ident;


void Identifier::print(const int level) {
  print_indent(level);
  std::cout << "Identifier " << name_ <<'\n';
}

void TypeIdentifier::print(const int level) {
  print_indent(level);
  std::cout << "Type Identifier " << '\n';
}
