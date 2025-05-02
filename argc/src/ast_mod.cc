#include "ast.hh"

using namespace ast;
using namespace ast::mod;

Module::Module (
  std::shared_ptr<ident::Identifier> id,
  std::vector<std::shared_ptr<func::Function>> fns,
  std::vector<std::shared_ptr<stmt::Statement>> stmts
) : identifier_(id), functions_(fns), statements_(stmts) {}

Module::Module (
  std::shared_ptr<ident::Identifier> id,
  std::vector<std::shared_ptr<stmt::Statement>> stmts
) : identifier_(id), statements_(stmts) {}

Module::Module (
  std::shared_ptr<ident::Identifier> id,
  std::vector<std::shared_ptr<func::Function>> fns
) : identifier_(id), functions_(fns) {}

void Module::print(const int level) {
  print_indent(level);
  std::cout << "Module \n";
  for(const auto& s : statements()){
    s->print(level + 1);
  }
  for(const auto& f : functions()){
    f->print(level + 1);
  }
}
