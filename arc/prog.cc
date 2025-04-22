#include <utility>

#include "include/ast.hh"



namespace ast::prog {


Program::Program(std::vector<std::shared_ptr<func::Function>> fns) : functions_(std::move(fns)) {

}

void Program::print(const int level) {
  std::cout << "Program " << '\n';
  for (const auto& fn : functions_) {
    fn->print(level + 1);
  }

}



}


