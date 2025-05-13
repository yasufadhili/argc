#include "semantic_analyser.hh"

using namespace ast;

void SemanticAnalyser::visit(mod::Module& m) {
  if (!m.identifier()) {
    REPORT_ERROR("Module with null identifier encountered", m.location());
    error_occurred_ = true;
    return;
  }
  std::string module_scope_name = "module_" + m.identifier()->name();
  symbol_table_->enter_scope(module_scope_name);
  
  auto module_symbol = std::make_shared<sym_table::Symbol>(
    m.identifier()->name(),
    sym_table::SymbolKind::MODULE,
    nullptr,  // Module doesn't have a type; type will be set later if needed
    true,     // Module is always defined
    sym_table::AccessModifier::PUBLIC,
    m.location().begin.line,
    m.location().begin.column,
    m.location().begin.filename ? m.location().begin.filename->c_str() : ""
  );

  symbol_table_->add_symbol(module_symbol);

  for (auto& f : m.functions()) {
    if (!f) {
      REPORT_ERROR("Null function encountered in module '" + m.identifier()->name() + "'", m.location());
      error_occurred_ = true;
      continue;
    }
    f->accept(*this);
  }

  symbol_table_->exit_scope();
}