#include <memory>

#include "include/ast.hh"

using namespace ast;

SemanticAnalyser::SemanticAnalyser()
  :
symbol_table_(sym_table::SymbolTable::get_instance()),
error_occurred_(false) {}


auto SemanticAnalyser::add_error(const std::string &msg)-> void {
  errors_.emplace_back(msg);
}

auto SemanticAnalyser::analyse(const std::shared_ptr<unit::TranslationUnit> &u) -> bool {
  error_occurred_ = false;
  u->accept(*this);
  return errors_.empty();
}
