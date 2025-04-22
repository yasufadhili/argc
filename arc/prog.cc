#include <iostream>
#include <utility>

#include "include/ast.hh"

namespace ast::prog {

  Program::Program(std::vector<std::shared_ptr<func::Function>> fns) : fns(std::move(fns)) {
    
  }

  void Program::print(const int level) {
    Node::print_indent(level);
    std::cout << "Program \n";
    
    Node::print_indent(level + 1);
    std::cout << "Functions:\n";
    for (const auto& f : fns) {
      if (f) f->print(level + 2);
    }
  }



}