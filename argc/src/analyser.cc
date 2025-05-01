#include <memory>

#include "include/ast.hh"

using namespace ast;

SemanticAnalyser::SemanticAnalyser()
  :
symbol_table_(sym_table::SymbolTable::get_instance()),
error_occurred_(false) {}


auto SemanticAnalyser::add_error(const std::string &msg, const yy::location& loc)-> void {
  errors_.emplace_back(msg, loc);
  // Also report to the central error handler
  error::DiagnosticHandler::instance().error(msg, loc);
}

auto SemanticAnalyser::analyse(const std::shared_ptr<unit::TranslationUnit> &u) -> bool {
  error_occurred_ = false;
  u->accept(*this);
  return errors_.empty();
}
