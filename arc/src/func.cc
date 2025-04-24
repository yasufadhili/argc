#include <utility>

#include "../include/ast.hh"


using namespace ast::func;

void FunctionBody::print(int level) {
  std::cout << "Function Body \n";
}


void Function::print(const int level) {
  print_indent(level);
  std::cout << "Function \n";
  name_->print(level + 1);

  if (return_type().has_value()) {
    if (std::holds_alternative<std::shared_ptr<ident::TypeIdentifier>>(return_type().value())) {
      std::get<std::shared_ptr<ident::TypeIdentifier>>(return_type().value())->print(level + 2);
    }
    else if (std::holds_alternative<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(return_type().value())) {
      std::cout << "...\n";
      for (const auto& rt: std::get<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(return_type().value())) {
        rt->print(level + 2);
      }
    }
  }

}



