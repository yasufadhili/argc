#include "include/ast.hh"

using namespace ast::ident;

Identifier::Identifier(std::string name)
  : name_(std::move(name))
{

}

void Identifier::print(int level) {
  print_indent(level);
  std::cout << "Identifier \n";
}

TypeIdentifier::TypeIdentifier(std::string name)
  : name_(std::move(name))
{

}

void TypeIdentifier::print(int level) {
  print_indent(level);
  std::cout << "Type Identifier \n";
}
