#include <utility>

#include "include/ast.hh"

using namespace ast::prog;

Module::Module(std::string name, std::vector<std::shared_ptr<func::Function> > functions)
  : name_(std::move(name)), functions_(std::move(functions)) {

}

void Module::print(const int level) {
  print_indent(level);
  std::cout << "Module \n";
  for (const auto& func : functions_) {
    func->print(level + 1);
  }
}

void Module::accept(Visitor &) {

}


