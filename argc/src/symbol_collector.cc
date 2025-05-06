#include "symbol_collector.hh"
#include "sym_table.hh"
#include "util_logger.hh"
#include <memory>
#include "error_handler.hh"

using namespace ast;

SymbolCollector::SymbolCollector() :
  Visitor(),
  symbol_table_(sym_table::SymbolTable::get_instance()),
  error_occurred_(false) {}

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
    nullptr,  // Module doesn't have a type; type will be set later if needed
    true,     // Module is always defined
    sym_table::AccessModifier::PUBLIC,
    m.location().begin.line,
    m.location().begin.column,
    m.location().begin.filename ? m.location().begin.filename->c_str() : ""
  );
  
  // Add module symbol to the table. Duplicate check will be done in semantic analysis.
  symbol_table_->add_symbol(module_symbol);
  
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
  
  // Create function symbol without resolving the return type here.
  // Return type resolution will happen in SemanticAnalyser.
  auto func_symbol = std::make_shared<sym_table::Symbol>(
    func.name()->name(),
    sym_table::SymbolKind::FUNC,
    nullptr, // Type will be set by SemanticAnalyser
    true,  // Function declaration means it's defined
    func.is_public() ? sym_table::AccessModifier::PUBLIC : sym_table::AccessModifier::PRIVATE,
    func.location().begin.line,
    func.location().begin.column,
    func.location().begin.filename ? func.location().begin.filename->c_str() : ""
  );
  
  // Add function symbol to the table. Duplicate check will be done in semantic analysis.
  symbol_table_->add_symbol(func_symbol);
  
  // Visit parameters first to add them to the symbol table within the function's scope
  std::vector<std::shared_ptr<sym_table::Type>> param_types; // This vector is no longer populated with resolved types here
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
    // Visit parameter to add its symbol to the current scope
    param->accept(*this);
    // Type lookup and error reporting for unknown parameter types are moved to SemanticAnalyser
  }
  
  // Visit the return type node, although SymbolCollector doesn't process its details
  if (func.return_type()) {
    func.return_type()->accept(*this);
  }
  
  // Visit the function body
  if (func.body()) {
    func.body()->accept(*this);
  }
  
  symbol_table_->exit_scope();
}

void SymbolCollector::visit(expr::FunctionCall& call) {
  if (call.function()) {
    call.function()->accept(*this);
  } else {
    REPORT_ERROR("Function call with null identifier", call.location());
    error_occurred_ = true;
  }
  for (auto& arg : call.arguments()) {
    if (arg) {
      arg->accept(*this);
    } else {
       REPORT_ERROR("Null argument in function call", call.location());
       error_occurred_ = true;
    }
  }
}

void SymbolCollector::visit(func::Parameter& param) {
  if (!param.identifier()) {
    REPORT_ERROR("Parameter with null identifier", param.location());
    error_occurred_ = true;
    return;
  }
  // Create parameter symbol without resolving the type here.
  // Type resolution will happen in SemanticAnalyser.
  auto param_symbol = std::make_shared<sym_table::Symbol>(
    param.identifier()->name(),
    sym_table::SymbolKind::PARAM,
    nullptr, // Type will be set by SemanticAnalyser
    true,    // Parameters are considered defined by the function signature
    sym_table::AccessModifier::PRIVATE, // Parameters are local to the function
    param.location().begin.line,
    param.location().begin.column,
    param.location().begin.filename ? param.location().begin.filename->c_str() : ""
  );
  // Add parameter symbol to the table. Duplicate name check within the function scope is done by add_symbol,
  // but reporting a semantic error for duplicate names is better handled in SemanticAnalyser.
  symbol_table_->add_symbol(param_symbol);

  // Visit the parameter type, although SymbolCollector doesn't process its details
  if (param.type()) {
    // Note: Visiting Type nodes is not standard for AST traversal for symbol collection.
    // The Type node in AST should represent the *syntax* of the type name.
    // The actual type lookup should happen later.
    // AST has sym_table::Type directly attached, we don't need to visit it here.
    // Since AST has ident::TypeIdentifier, visiting it is okay but won't add to symbol table.
  }
}

void SymbolCollector::visit(stmt::VariableDeclaration& vd) {
  if (!vd.identifier()) {
    REPORT_ERROR("Variable declaration with null identifier", vd.location());
    error_occurred_ = true;
    return;
  }

  // Create variable/const symbol without resolving the type here.
  // Type resolution will happen in SemanticAnalyser.
  auto var_symbol = std::make_shared<sym_table::Symbol>(
    vd.identifier()->name(),
    vd.is_const() ? sym_table::SymbolKind::CONST : sym_table::SymbolKind::VAR,
    nullptr, // Type will be set by SemanticAnalyser
    true,    // Variable declaration means it's defined
    sym_table::AccessModifier::PRIVATE, // Variables are local to the scope
    vd.location().begin.line,
    vd.location().begin.column,
    vd.location().begin.filename ? vd.location().begin.filename->c_str() : ""
  );

  // Add variable symbol to the table. Duplicate check is better handled in SemanticAnalyser.
  symbol_table_->add_symbol(var_symbol);

  // Visit the declared type node, although SymbolCollector doesn't process its details
  if (vd.type()) {
     // As with parameters, visiting the sym_table::Type node directly is unusual for AST traversal.
     // We are collecting symbols, not type details here. The Type object is likely already resolved
     // by a parser phase and attached to the AST node. We rely on SemanticAnalyser to validate it.
  }

  // Visit the initializer expression if present
  if (vd.initialiser()) {
    (*vd.initialiser())->accept(*this);
  }
}

void SymbolCollector::visit(expr::Variable& var) {
  if (!var.identifier()) {
     REPORT_ERROR("Variable expression with null identifier", var.location());
     error_occurred_ = true;
     return;
  }
  if (auto symbol = symbol_table_->lookup_symbol(var.identifier()->name())) {
    symbol->set_used(true);
  }
  else {
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
    if (stmt) {
      stmt->accept(*this);
    } else {
      REPORT_ERROR("Null statement in block", b.location());
      error_occurred_ = true;
    }
  }
  
  symbol_table_->exit_scope();
}

// Default implementations for nodes that don't need special handling for symbol collection
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
  if (ps.expression()) {
    ps.expression()->accept(*this);
  } else {
     REPORT_ERROR("Print statement requires an expression", ps.location());
     error_occurred_ = true;
  }
}
void SymbolCollector::visit(stmt::Assignment& s) {
  if (s.target()) {
    s.target()->accept(*this); // Visit the target identifier (should be a Variable node)
  } else {
    REPORT_ERROR("Assignment with null target", s.location());
    error_occurred_ = true;
  }
  if (s.value()) {
    s.value()->accept(*this);
  } else {
    REPORT_ERROR("Assignment with null value", s.location());
    error_occurred_ = true;
  }
}
void SymbolCollector::visit(expr::Expression&) {}
void SymbolCollector::visit(expr::Literal&) {}
void SymbolCollector::visit(func::Body& b) {
  for (auto& stmt : b.statements()) {
    if (stmt) {
      stmt->accept(*this);
    } else {
      REPORT_ERROR("Null statement in function body", b.location());
      error_occurred_ = true;
    }
  }
}
void SymbolCollector::visit(func::ReturnTypeInfo&) {}
void SymbolCollector::visit(func::SingleReturnType&) {}
void SymbolCollector::visit(func::MultipleReturnType&) {}

bool SymbolCollector::successful() const {
    return !error_occurred_;
}

