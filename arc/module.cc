#include <iostream>
#include <utility>

#include "include/ast.hh"

namespace ast::module {

  Module::Module(std::vector<stmt::Statement *> stmts, std::vector<func::Function *> fns)
    : statements(std::move(stmts)), functions(std::move(fns))
  {

  }

  Module::~Module() {
    for (const auto &stmt : statements) {
      delete stmt;
    }
    for (const auto &fn : functions) {
      delete fn;
    }
  }

  void Module::print(int level) {
    Node::print_indent(level);
    std::cout << "Module\n";
    
    Node::print_indent(level + 1);
    std::cout << "Statements:\n";
    for (const auto stmt : statements) {
      if (stmt) stmt->print(level + 2);
    }
    
    Node::print_indent(level + 1);
    std::cout << "Functions:\n";
    for (const auto fn : functions) {
      if (fn) fn->print(level + 2);
    }
  }

}