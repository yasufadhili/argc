#include <iostream>
#include <utility>

#include "ast.hh"

namespace ast::prog {


  Program::Program(std::vector<module::Module *> modules) : modules(std::move(modules)) {
  }

  Program::~Program() {
    for (const auto module : modules) {
      delete module;
    }
  }



  void Program::print(const int level) {
    Node::print_indent(level);
    std::cout << "Program\n";
    
    Node::print_indent(level + 1);
    std::cout << "Modules:\n";
    for (const auto module : modules) {
      if (module) module->print(level + 2);
    }
  }



}