#include <utility>

#include "ast.hh"



using namespace ast::prog;


Program::Program(std::vector<std::shared_ptr<Module>> modules) : modules_(std::move(modules)) {

}

void Program::print(const int level) {
  std::cout << "Program " << '\n';
  for (const auto& m : modules_) {
    m->print(level + 1);
  }

}

void Program::accept(Visitor &v) {
  v.emit("  .section .data");
  v.emit("  .global main");
  v.emit("  .type main, @function");
}


