#include <iostream>
#include <utility>

#include "include/ast.hh"

namespace ast::prog {

  Program::Program(std::vector<std::shared_ptr<expr::Expression>> exps) : expressions(std::move(exps)) {
    
  }

  void Program::print(const int level) {
    Node::print_indent(level);
    std::cout << "Program\n";
    
    Node::print_indent(level + 1);
    std::cout << "Expressions:\n";
    for (const auto& exp : expressions) {
      if (exp) exp->print(level + 2);
    }
  }



}