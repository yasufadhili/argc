#include <utility>

#include "ast.hh"

using namespace ast::prog;

Module::Module(std::string name, std::vector<std::shared_ptr<func::Function>> functions)
  : name_(std::move(name)), functions_(std::move(functions)) {}


void Module::accept(Visitor& v) {
  if (name_ == "main") {
    v.emit("_start:");
    v.emit("  call main");
    v.emit("  mov $60, %rax"); 
    v.emit("  xor %rdi, %rdi");
    v.emit("  syscall");
  }
  for (auto const& f : functions_) {
    f->accept(v);
  }
}

void Module::print(const int level) {
  print_indent(level);
  std::cout << "Module: " << name_ << std::endl;
  for (const auto& func : functions_) {
    func->print(level + 1);
  }
}

void Module::add_function(const std::shared_ptr<func::Function>& func) {
  functions_.push_back(func);
}


