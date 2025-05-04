#include "include/ast.hh"
#include "sym_table.hh"
#include <memory>
#include "error_handler.hh"

using namespace ast;

SymbolCollector::SymbolCollector() :
  Visitor(),
  symbol_table_(sym_table::SymbolTable::get_instance()),
  error_occurred_(false) {}

void SymbolCollector::visit(unit::TranslationUnit& tu) {
  symbol_table_->enter_scope("global");
  
  for (auto& m : tu.modules()) {
    if (!m) {
      REPORT_ERROR("Null module encountered in translation unit", tu.location());
      error_occurred_ = true;
      continue;
    }
    m->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(mod::Module& m) {
  if (!m.identifier()) {
    REPORT_ERROR("Module with null identifier encountered", m.location());
    error_occurred_ = true;
    return;
  }
  if (config_.verbose) {
    LOG_INFO("Collecting module symbols");
  }
  
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
    m.location().begin.filename ? m.location().begin.filename->c_str() : ""
  );
  
  if (!symbol_table_->add_symbol(module_symbol)) {
    REPORT_ERROR("Duplicate module declaration: '" + m.identifier()->name() + "'", m.location());
    error_occurred_ = true;
    symbol_table_->exit_scope();
    return;
  }
  
  for (auto& func : m.functions()) {
    if (!func) {
      REPORT_ERROR("Null function encountered in module '" + m.identifier()->name() + "'", m.location());
      error_occurred_ = true;
      continue;
    }
    func->accept(*this);
  }

  for (auto& stmt : m.statements()) {
    if (!stmt) {
      REPORT_ERROR("Null statement encountered in module '" + m.identifier()->name() + "'", m.location());
      error_occurred_ = true;
      continue;
    }
    stmt->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(func::Function& func) {
  if (!func.name()) {
    REPORT_ERROR("Function with null identifier encountered", func.location());
    error_occurred_ = true;
    return;
  }
  std::string func_scope_name = "func_" + func.name()->name();
  symbol_table_->enter_scope(func_scope_name);
  
  std::vector<std::shared_ptr<sym_table::Type>> param_types;
  for (const auto& param : func.parameters()) {
    if (!param) {
      REPORT_ERROR("Null parameter encountered in function '" + func.name()->name() + "'", func.location());
      error_occurred_ = true;
      continue;
    }
    if (!param->identifier()) {
      REPORT_ERROR("Parameter with null identifier in function '" + func.name()->name() + "'", param->location());
      error_occurred_ = true;
      continue;
    }
    param->accept(*this);
    auto type = symbol_table_->lookup_type(param->type()->name());
    if (!type) {
      REPORT_ERROR("Unknown type for parameter '" + param->identifier()->name() + "' in function '" + func.name()->name() + "'", param->location());
      error_occurred_ = true;
    }
    param_types.push_back(type);
  }
  
  std::shared_ptr<sym_table::Type> return_type;
  if (auto single_ret = std::dynamic_pointer_cast<func::SingleReturnType>(func.return_type())) {
    if (!single_ret->identifier()) {
      REPORT_ERROR("Return type with null identifier in function '" + func.name()->name() + "'", func.location());
      error_occurred_ = true;
      return_type = nullptr;
    } else {
      return_type = symbol_table_->lookup_type(single_ret->identifier()->name());
      if (!return_type) {
        REPORT_ERROR("Unknown return type '" + single_ret->identifier()->name() + "' for function '" + func.name()->name() + "'", func.location());
        error_occurred_ = true;
      }
    }
  } else if (auto multi_ret = std::dynamic_pointer_cast<func::MultipleReturnType>(func.return_type())) {
    REPORT_ERROR("Multiple return types are not supported yet", func.location());
    error_occurred_ = true;
    return_type = nullptr;
  } else {
    return_type = nullptr; // void
  }
  
  auto func_symbol = std::make_shared<sym_table::Symbol>(
    func.name()->name(),
    sym_table::SymbolKind::FUNC,
    return_type,
    true,  // Function is defined
    func.is_public() ? sym_table::AccessModifier::PUBLIC : sym_table::AccessModifier::PRIVATE,
    func.location().begin.line,
    func.location().begin.column,
    func.location().begin.filename ? func.location().begin.filename->c_str() : ""
  );
  
  if (!symbol_table_->add_symbol(func_symbol)) {
    REPORT_ERROR("Duplicate function declaration: '" + func.name()->name() + "'", func.location());
    error_occurred_ = true;
    symbol_table_->exit_scope();
    return;
  }
  
  for (const auto& param : func.parameters()) {
    if (!param || !param->identifier()) continue;
    auto param_type = symbol_table_->lookup_type(param->type()->name());
    auto param_symbol = std::make_shared<sym_table::Symbol>(
      param->identifier()->name(),
      sym_table::SymbolKind::PARAM,
      param_type,
      true,
      sym_table::AccessModifier::PRIVATE,
      param->location().begin.line,
      param->location().begin.column,
      param->location().begin.filename ? param->location().begin.filename->c_str() : ""
    );
    if (!symbol_table_->add_symbol(param_symbol)) {
      REPORT_ERROR("Duplicate parameter name: '" + param->identifier()->name() + "' in function '" + func.name()->name() + "'", param->location());
      error_occurred_ = true;
    }
  }
  
  if (func.body()) {
    func.body()->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(expr::FunctionCall&) {}

void SymbolCollector::visit(func::Parameter& param) {
  // No action needed here; handled in function
}

void SymbolCollector::visit(stmt::VariableDeclaration& vd) {
  auto type = vd.type();
  if (!type) {
    REPORT_ERROR("Unknown type for variable '" + vd.identifier()->name() + "'", vd.location());
    error_occurred_ = true;
    return;
  }
  
  auto var_symbol = std::make_shared<sym_table::Symbol>(
    vd.identifier()->name(),
    vd.is_const() ? sym_table::SymbolKind::CONST : sym_table::SymbolKind::VAR,
    type,
    true,
    sym_table::AccessModifier::PRIVATE,
    vd.location().begin.line,
    vd.location().begin.column,
    vd.location().begin.filename ? vd.location().begin.filename->c_str() : ""
  );
  
  if (!symbol_table_->add_symbol(var_symbol)) {
    REPORT_ERROR("Duplicate variable declaration: '" + vd.identifier()->name() + "'", vd.location());
    error_occurred_ = true;
    return;
  }
  
  if (vd.initialiser()) {
    (*vd.initialiser())->accept(*this);
  }
}

void SymbolCollector::visit(expr::Variable& var) {
  if (auto symbol = symbol_table_->lookup_symbol(var.identifier()->name())) {
    symbol->set_used(true);
  } else {
    REPORT_ERROR("Use of undeclared variable '" + var.identifier()->name() + "'", var.location());
    error_occurred_ = true;
  }
}

void SymbolCollector::visit(expr::Binary& e) {
  if (e.lhs()) {
    e.lhs()->accept(*this);
  } else {
    REPORT_ERROR("Null left-hand side in binary expression", e.location());
    error_occurred_ = true;
  }
  if (e.rhs()) {
    e.rhs()->accept(*this);
  } else {
    REPORT_ERROR("Null right-hand side in binary expression", e.location());
    error_occurred_ = true;
  }
}

void SymbolCollector::visit(expr::Unary& e) {
  if (e.operand()) {
    e.operand()->accept(*this);
  } else {
    REPORT_ERROR("Null operand in unary expression", e.location());
    error_occurred_ = true;
  }
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

bool SymbolCollector::successful() const {
    return !error_occurred_;
}

