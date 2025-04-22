#include "include/ast.hh"


using namespace ast::ident;


Identifier::Identifier(const std::string &n) : name_(n) {};
Identifier::~Identifier() = default;

TypeIdentifier::TypeIdentifier(const std::string &n) : name_(n) {};
TypeIdentifier::~TypeIdentifier() = default;

void Identifier::print(const int level) {
  std::cout << "Identifier " << '\n';
}

void TypeIdentifier::print(int level) {
  std::cout << "Type Identifier " << '\n';
}
