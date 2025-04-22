#include <iostream>
#include <utility>

#include "include/ast.hh"

namespace ast::prog {

  Program::Program(std::vector<std::shared_ptr<stmt::Statement>> stmts) : statements(std::move(stmts)) {
    
  }

  void Program::print(const int level) {
    Node::print_indent(level);
    std::cout << "Program\n";
    
    Node::print_indent(level + 1);
    std::cout << "Statements:\n";
    for (const auto& st : statements) {
      if (st) st->print(level + 2);
    }
  }



}