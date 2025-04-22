#include <iostream>
#include <utility>

#include "include/ast.hh"


namespace ast::func {

  Function::Function(std::string n, std::shared_ptr<stmt::Block> b) : name(n), body(std::move(b)) {

  }

  void Function::print(const int level) {
    Node::print_indent(level);
    std::cout << "Function: " << name << std::endl;
    if (body) body->print(level + 1);
  }


}
