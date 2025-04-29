#include <memory>

#include "include/ast.hh"

using namespace ast;

auto SemanticAnalyser::add_error(const std::string &msg)-> void {
  errors_.emplace_back(msg);
}

auto SemanticAnalyser::analyse(std::shared_ptr<unit::TranslationUnit> &) -> bool {
  return errors_.empty();
}

void SemanticAnalyser::visit(std::shared_ptr<unit::TranslationUnit> &) {

}
