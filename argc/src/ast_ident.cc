#include "include/ast.hh"

using namespace ast::ident;

Identifier::Identifier(std::string name)
  : name_(std::move(name))
{

}

void Identifier::accept(SemanticAnalyser &) {

}

void Identifier::accept(CodeGenerator &) {

}

void Identifier::print(int level) {
  print_indent(level);
  std::cout << "Identifier \n";
}
