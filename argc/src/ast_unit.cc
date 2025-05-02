#include <iostream>
#include <memory>
#include "include/ast.hh"

using namespace ast::unit;

TranslationUnit::TranslationUnit(std::vector<std::shared_ptr<mod::Module> > m)
  : modules_(std::move(m))
{

}

void TranslationUnit::print(const int level) {
  print_indent(level);
  std::cout << "Translation Unit\n";
  for (const auto& m : modules_) {
    m->print(level + 1);
  }
}
