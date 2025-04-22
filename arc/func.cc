#include <iostream>
#include <utility>

#include "include/ast.hh"


namespace ast::func {

  Function::Function(std::string n, std::optional<std::vector<std::shared_ptr<Parameter>>> p, std::string r, std::shared_ptr<stmt::Block> b)
    : name(std::move(n)), parameters(std::move(p)), return_type(std::move(r)), body(std::move(b))
  {

  }


  void Function::print(const int level) {
    Node::print_indent(level);
    std::cout << "Function: " << name << std::endl;
    if (body) body->print(level + 1);
  }


}
