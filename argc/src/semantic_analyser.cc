#include <memory>

#include "include/ast.hh"
#include "sym_table.hh"
#include "error_handler.hh"

using namespace ast;

void SemanticAnalyser::visit(unit::TranslationUnit& tu) {
  for (auto& m : tu.modules()) {
    m->accept(*this);
  }
}

void SemanticAnalyser::visit(mod::Module& m) {
  symbol_table_->enter_scope(m.identifier()->name());
  
  // Process functions first to allow forward references
  for (auto& f : m.functions()) {
    f->accept(*this);
  }

  for (auto& s : m.statements()) {
    s->accept(*this);
  }

  symbol_table_->exit_scope();
}

void SemanticAnalyser::visit(func::Function& f) {
  symbol_table_->enter_scope(f.name()->name());

  // Set current return type for checking return statements
  if (auto single_ret = std::dynamic_pointer_cast<func::SingleReturnType>(f.return_type())) {
    auto ret_type = symbol_table_->lookup_type(single_ret->identifier()->name());
    if (!ret_type) {
      REPORT_ERROR("Unknown return type '" + single_ret->identifier()->name() + "' for function '" + f.name()->name() + "'", f.location());
      error_occurred_ = true;
    }
    current_return_type_ = ret_type;
  } else if (auto multi_ret = std::dynamic_pointer_cast<func::MultipleReturnType>(f.return_type())) {
    
    REPORT_ERROR("Multiple return types are not supported yet", f.location());
    error_occurred_ = true;
    current_return_type_ = nullptr;
  } else {
    current_return_type_ = nullptr; // void
  }

  if (f.body()) {
    f.body()->accept(*this);
  }

  symbol_table_->exit_scope();
  current_return_type_ = nullptr;
}

void SemanticAnalyser::visit(func::FunctionCall&){

}

void SemanticAnalyser::visit(func::Parameter& param) {
  // No semantic checks needed here (already checked in symbol collection)
}

void SemanticAnalyser::visit(func::ReturnTypeInfo&) {}
void SemanticAnalyser::visit(func::MultipleReturnType&) {}
void SemanticAnalyser::visit(func::SingleReturnType&) {}

void SemanticAnalyser::visit(func::Body& body) {
  for (auto& stmt : body.statements()) {
    if (stmt) stmt->accept(*this);
  }
}

void SemanticAnalyser::visit(ident::Identifier&) {}
void SemanticAnalyser::visit(ident::TypeIdentifier&) {}

void SemanticAnalyser::visit(stmt::Statement&) {}
void SemanticAnalyser::visit(stmt::Empty&) {}

void SemanticAnalyser::visit(stmt::Block& block) {
  symbol_table_->enter_scope("block");
  for (auto& stmt : block.statements()) {
    if (stmt) stmt->accept(*this);
  }
  symbol_table_->exit_scope();
}

void SemanticAnalyser::visit(stmt::Return& ret) {
  if (!current_return_type_) {
    if (ret.expression()) {
      REPORT_ERROR("Return statement with a value in a void function", ret.location());
      error_occurred_ = true;
    }
    return;
  }
  if (!ret.expression()) {
    REPORT_ERROR("Missing return value in non-void function", ret.location());
    error_occurred_ = true;
    return;
  }
  auto expr = *ret.expression();
  expr->accept(*this);
  if (!expr->type()) return;
  if (!expr->type()->is_compatible_with(*current_return_type_)) {
    REPORT_ERROR("Return type mismatch: expected '" + current_return_type_->to_string() + "', got '" + expr->type()->to_string() + "'", ret.location());
    error_occurred_ = true;
  }
}

void SemanticAnalyser::visit(stmt::Print& ps) {
  if (ps.expression()) {
    ps.expression()->accept(*this);
  }
}

void SemanticAnalyser::visit(stmt::VariableDeclaration& vd) {
  auto type = vd.type();
  if (!type) {
    REPORT_ERROR("Unknown type for variable '" + vd.identifier()->name() + "'", vd.location());
    error_occurred_ = true;
    return;
  }
  // Check initialiser type
  if (vd.initialiser()) {
    auto expr = *vd.initialiser();
    expr->accept(*this);
    if (expr->type() && !expr->type()->can_implicitly_cast_to(*type)) {
      REPORT_ERROR("Type mismatch in initialiser for variable '" + vd.identifier()->name() + "': expected '" + type->to_string() + "', got '" + expr->type()->to_string() + "'", vd.location());
      error_occurred_ = true;
    }
  }
}

void SemanticAnalyser::visit(stmt::Assignment& assign) {
  // Check variable exists
  auto symbol = symbol_table_->lookup_symbol(assign.target()->name());
  if (!symbol) {
    REPORT_ERROR("Assignment to undeclared variable '" + assign.target()->name() + "'", assign.location());
    error_occurred_ = true;
    return;
  }
  if (symbol->get_kind() == sym_table::SymbolKind::CONST) {
    REPORT_ERROR("Cannot assign to constant variable '" + assign.target()->name() + "'", assign.location());
    error_occurred_ = true;
    return;
  }
  // Check value type
  assign.value()->accept(*this);
  auto value_type = assign.value()->type();
  auto var_type = symbol->get_type();
  if (value_type && var_type && !value_type->can_implicitly_cast_to(*var_type)) {
    REPORT_ERROR("Type mismatch in assignment to variable '" + assign.target()->name() + "': expected '" + var_type->to_string() + "', got '" + value_type->to_string() + "'", assign.location());
    error_occurred_ = true;
  }
}

void SemanticAnalyser::visit(expr::Expression&) {}

void SemanticAnalyser::visit(expr::Literal& lit) {
  // Set type based on literal value
  std::visit([&](auto&& value) {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, int64_t>) {
      lit.set_type(symbol_table_->lookup_type("i64"));
    } else if constexpr (std::is_same_v<T, double>) {
      lit.set_type(symbol_table_->lookup_type("f64"));
    } else if constexpr (std::is_same_v<T, bool>) {
      lit.set_type(symbol_table_->lookup_type("bool"));
    } else if constexpr (std::is_same_v<T, u_int64_t>) {
      lit.set_type(symbol_table_->lookup_type("u64"));
    }
  }, lit.value());
}

void SemanticAnalyser::visit(expr::Binary& bin) {
  if (bin.lhs()) bin.lhs()->accept(*this);
  if (bin.rhs()) bin.rhs()->accept(*this);
  auto lhs_type = bin.lhs() ? bin.lhs()->type() : nullptr;
  auto rhs_type = bin.rhs() ? bin.rhs()->type() : nullptr;
  if (!lhs_type || !rhs_type) return;
  // Type checking for binary operations
  if (std::holds_alternative<BinaryOp>(bin.op())) {
    auto op = std::get<BinaryOp>(bin.op());
    switch (op) {
      case BinaryOp::ADD:
      case BinaryOp::SUB:
      case BinaryOp::MUL:
      case BinaryOp::DIV:
      case BinaryOp::MOD:
        if (!lhs_type->is_numeric_type() || !rhs_type->is_numeric_type()) {
          REPORT_ERROR("Operands of arithmetic operator must be numeric", bin.location());
          error_occurred_ = true;
        }
        bin.set_type(lhs_type->get_common_type(*lhs_type, *rhs_type));
        break;
      case BinaryOp::B_AND:
      case BinaryOp::B_OR:
        if (!lhs_type->is_integral_type() || !rhs_type->is_integral_type()) {
          REPORT_ERROR("Operands of bitwise operator must be integral", bin.location());
          error_occurred_ = true;
        }
        bin.set_type(lhs_type->get_common_type(*lhs_type, *rhs_type));
        break;
      case BinaryOp::L_AND:
      case BinaryOp::L_OR:
        if (!lhs_type->is_compatible_with(*symbol_table_->lookup_type("bool")) ||
            !rhs_type->is_compatible_with(*symbol_table_->lookup_type("bool"))) {
          REPORT_ERROR("Operands of logical operator must be boolean", bin.location());
          error_occurred_ = true;
        }
        bin.set_type(symbol_table_->lookup_type("bool"));
        break;
      case BinaryOp::NONE:
        break;
    }
  } else if (std::holds_alternative<RelationalOp>(bin.op())) {
    // All relational ops result in bool
    bin.set_type(symbol_table_->lookup_type("bool"));
  }
}

void SemanticAnalyser::visit(expr::Unary& un) {
  if (un.operand()) un.operand()->accept(*this);
  auto operand_type = un.operand() ? un.operand()->type() : nullptr;
  if (!operand_type) return;
  switch (un.op()) {
    case UnaryOp::NEG:
      if (!operand_type->is_numeric_type()) {
        REPORT_ERROR("Operand of unary minus must be numeric", un.location());
        error_occurred_ = true;
      }
      un.set_type(operand_type);
      break;
    case UnaryOp::B_NOT:
      if (!operand_type->is_integral_type()) {
        REPORT_ERROR("Operand of bitwise not must be integral", un.location());
        error_occurred_ = true;
      }
      un.set_type(operand_type);
      break;
    case UnaryOp::L_NOT:
      if (!operand_type->is_compatible_with(*symbol_table_->lookup_type("bool"))) {
        REPORT_ERROR("Operand of logical not must be boolean", un.location());
        error_occurred_ = true;
      }
      un.set_type(symbol_table_->lookup_type("bool"));
      break;
  }
}

void SemanticAnalyser::visit(expr::Variable& var) {
  auto symbol = symbol_table_->lookup_symbol(var.identifier()->name());
  if (!symbol) {
    REPORT_ERROR("Use of undeclared variable '" + var.identifier()->name() + "'", var.location());
    error_occurred_ = true;
    return;
  }
  var.set_type(symbol->get_type());
}