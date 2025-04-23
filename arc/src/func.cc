#include <utility>

#include "include/ast.hh"


using namespace ast::func;

Function::Function(std::shared_ptr<ident::Identifier> i) : identifier_(std::move(i)) {};

Function::~Function() = default;

void Function::print(const int level) {
  print_indent(level);
  std::cout << "Function \n";
  identifier_->print(level + 1);

  if (std::holds_alternative<std::shared_ptr<ident::TypeIdentifier>>(function_return_)) {
    std::get<std::shared_ptr<ident::TypeIdentifier>>(function_return_)->print(level + 2);
  }

  else if (std::holds_alternative<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(function_return_)) {
    std::cout << "...\n";
    for (const auto& rt: std::get<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(function_return_)) {
      rt->print(level + 2);
    }
  }

}



