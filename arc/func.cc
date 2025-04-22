#include <utility>

#include "include/ast.hh"


using namespace ast::func;

Function::Function(std::shared_ptr<ident::Identifier> i) : identifier_(std::move(i)) {};

Function::~Function() = default;

void Function::print(const int level) {
  print_indent(level);
  std::cout << "Function \n";
  identifier_->print(level + 1);
}



