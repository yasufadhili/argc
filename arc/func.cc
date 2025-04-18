#include <iostream>

#include "ast.hh"


namespace ast::func {

  Function::Function(stmt::Block *b) : body(b) {

  }

  Function::~Function() {
    delete body;
  }


  void Function::print(int level) {
    Node::print_indent(level);
    std::cout << "Function\n";
    if (body) body->print(level + 1);
  }


}
