#include <utility>

#include "include/ast.hh"

using namespace ast::prog;

Module::Module(std::string name, std::vector<std::shared_ptr<func::Function>> functions)
  : name_(std::move(name)), functions_(std::move(functions)) {}


void Module::accept(Visitor& v) {

}

void Module::print(int level) {
  std::cout << std::string(level * 2, ' ') << "Module: " << name_ << std::endl;
  for (const auto& func : functions_) {
    func->print(level + 1);
  }
}

void Module::add_function(const std::shared_ptr<func::Function>& func) {
  functions_.push_back(func);
}


