#include "include/ast.hh"
#include "sym_table.hh"
#include <memory>

using namespace ast;

void SymbolCollector::visit(unit::TranslationUnit& tu) {
  symbol_table_->enter_scope("global");
  
  for (auto& m : tu.modules()) {
    m->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(mod::Module& m) {
  std::string module_scope_name = "module_" + m.identifier()->name();
  symbol_table_->enter_scope(module_scope_name);
  
  auto module_symbol = std::make_shared<sym_table::Symbol>(
    m.identifier()->name(),
    sym_table::SymbolKind::MODULE,
    nullptr,  // Module doesn't have a type
    true,     // Module is always defined
    sym_table::AccessModifier::PUBLIC,
    m.location().begin.line,
    m.location().begin.column,
    m.location().begin.filename->c_str()
  );
  
  if (!symbol_table_->add_symbol(module_symbol)) {
    error_occurred_ = true;
    // TODO: Add proper error reporting
    return;
  }
  
  for (auto& func : m.functions()) {
    func->accept(*this);
  }

  for (auto& stmt : m.statements()) {
    stmt->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(func::Function& func) {
  std::string func_scope_name = "func_" + func.name()->name();
  symbol_table_->enter_scope(func_scope_name);
  
  std::vector<std::shared_ptr<sym_table::Type>> param_types;
  for (const auto& param : func.parameters()) {
    param->accept(*this);
    if (auto type = symbol_table_->lookup_type(param->type()->name())) {
      param_types.push_back(type);
    } else {
      error_occurred_ = true;
      // TODO: Add proper error reporting for unknown type
    }
  }
  
  std::shared_ptr<sym_table::Type> return_type;
  if (auto single_ret = std::dynamic_pointer_cast<func::SingleReturnType>(func.return_type())) {
    return_type = symbol_table_->lookup_type(single_ret->identifier()->name());
  } else if (auto multi_ret = std::dynamic_pointer_cast<func::MultipleReturnType>(func.return_type())) {
    // TODO: Handle multiple return types
    error_occurred_ = true;
  }
  
  if (!return_type) {
    error_occurred_ = true;
    // TODO: Add proper error reporting for unknown return type
  }
  
  auto func_symbol = std::make_shared<sym_table::Symbol>(
    func.name()->name(),
    sym_table::SymbolKind::FUNC,
    return_type,
    true,  // Function is defined
    func.is_public() ? sym_table::AccessModifier::PUBLIC : sym_table::AccessModifier::PRIVATE,
    func.location().begin.line,
    func.location().begin.column,
    func.location().begin.filename->c_str()
  );
  
  if (!symbol_table_->add_symbol(func_symbol)) {
    error_occurred_ = true;
    // TODO: Add proper error reporting
    return;
  }
  
  func.body()->accept(*this);
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(func::Parameter& param) {
  
  auto param_symbol = std::make_shared<sym_table::Symbol>(
    param.identifier()->name(),
    sym_table::SymbolKind::PARAM,
    symbol_table_->lookup_type(param.type()->name()),
    true,  // Parameters are always defined
    sym_table::AccessModifier::PRIVATE,
    param.location().begin.line,
    param.location().begin.column,
    param.location().begin.filename->c_str()
  );
  
  if (!symbol_table_->add_symbol(param_symbol)) {
    error_occurred_ = true;
    // TODO: Add proper error reporting
  }
}

void SymbolCollector::visit(stmt::VariableDeclaration& vd) {
  
  auto var_symbol = std::make_shared<sym_table::Symbol>(
    vd.identifier()->name(),
    vd.is_const() ? sym_table::SymbolKind::CONST : sym_table::SymbolKind::VAR,
    vd.type(),
    true,  // Variable is defined
    sym_table::AccessModifier::PRIVATE,
    vd.location().begin.line,
    vd.location().begin.column,
    vd.location().begin.filename->c_str()
  );
  
  if (!symbol_table_->add_symbol(var_symbol)) {
    error_occurred_ = true;
    // TODO: Add proper error reporting
    return;
  }
  
  // Process initializer if present
  if (vd.initialiser()) {
    (*vd.initialiser())->accept(*this);
  }
}

void SymbolCollector::visit(expr::Variable& var) {
  
  if (auto symbol = symbol_table_->lookup_symbol(var.identifier()->name())) {
    symbol->set_used(true);
  } else {
    error_occurred_ = true;
    // TODO: Add proper error reporting for undefined variable
  }
}

void SymbolCollector::visit(expr::Binary& e) {
  e.lhs()->accept(*this);
  e.rhs()->accept(*this);
}

void SymbolCollector::visit(expr::Unary& e) {
  e.operand()->accept(*this);
}

void SymbolCollector::visit(stmt::Block& b) {
  symbol_table_->enter_scope("block");
  
  for (auto& stmt : b.statements()) {
    stmt->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

// Default implementations for nodes that don't need special handling
void SymbolCollector::visit(ident::Identifier&) {}
void SymbolCollector::visit(ident::TypeIdentifier&) {}
void SymbolCollector::visit(stmt::Statement&) {}
void SymbolCollector::visit(stmt::Empty&) {}
void SymbolCollector::visit(stmt::Return& rs) {
  if (rs.expression()) {
    (*rs.expression())->accept(*this);
  }
}
void SymbolCollector::visit(stmt::Print& ps) {
  ps.expression()->accept(*this);
}
void SymbolCollector::visit(stmt::Assignment& s) {
  s.target()->accept(*this);
  s.value()->accept(*this);
}
void SymbolCollector::visit(expr::Expression&) {}
void SymbolCollector::visit(expr::Literal&) {}
void SymbolCollector::visit(func::Body& b) {
  for (auto& stmt : b.statements()) {
    stmt->accept(*this);
  }
}
void SymbolCollector::visit(func::ReturnTypeInfo&) {}
void SymbolCollector::visit(func::SingleReturnType&) {}
void SymbolCollector::visit(func::MultipleReturnType&) {}

