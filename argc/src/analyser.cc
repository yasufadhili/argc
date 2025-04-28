#include "analyser.hh"
#include "ast.hh"
#include "checker.hh"
#include "symbols.hh"
#include "util_logger.hh"
#include <memory>

using namespace analyser;

SymbolCollector::SymbolCollector() {

}

SemanticAnalyser::SemanticAnalyser()
  : symbol_table_(sym::SymbolTable::get_instance() ),
    error_occured_(false),
    current_function_return_type(nullptr)
{}

auto SemanticAnalyser::analyse(Node &node) -> bool {
  error_occured_ = false;
  node.accept(*this);
  return !error_occured_;
}

void SemanticAnalyser::visit(std::shared_ptr<prog::Program>& p) {
  for (auto& m : p->modules()) {
    visit(m);
  }
}

void SemanticAnalyser::visit(std::shared_ptr<prog::Module>& m) {
  auto module_sym = symbol_table_->lookup_symbol(m->name());
  for (auto& fn : m->functions()) {
    visit(fn);
  }
}

void SemanticAnalyser::visit(std::shared_ptr<func::Function>& f) {
  std::string fn_name { f->name()->name() };
  auto fn_symbol { symbol_table_->lookup_symbol(f->name()->name()) };
  if (fn_symbol->get_is_defined() && f->body()!=nullptr) {
    LOG_ERROR(fn_name + " is already defined");
    return;
  }

  if (f->body() != nullptr) {
    fn_symbol->set_defined(true);
  }

  // Enter function scope for params and body
  symbol_table_->enter_scope(fn_name);

  //f->body()->accept(*this);
  visit(f->body());

  symbol_table_->exit_scope();

}

void SemanticAnalyser::visit(std::shared_ptr<stmt::Block>& b) {
  symbol_table_->enter_scope();
  
  symbol_table_->exit_scope();
}

void SemanticAnalyser::visit(std::shared_ptr<expr::rel::Relational>& e) {
  visit(e->lhs());
  visit(e->rhs());
  
  auto lhs_type = e->lhs()->type();
  auto rhs_type = e->rhs()->type();
  
  // Convert relational type to string operator
  std::string op;
  switch (e->type()) {
    case expr::rel::RelationalType::EQ: op = "=="; break;
    case expr::rel::RelationalType::NEQ: op = "!="; break;
    case expr::rel::RelationalType::GT: op = ">"; break;
    case expr::rel::RelationalType::LT: op = "<"; break;
    case expr::rel::RelationalType::GEQ: op = ">="; break;
    case expr::rel::RelationalType::LEQ: op = "<="; break;
    case expr::rel::RelationalType::NONE: 
      std::string err = "Invalid relational operator";
      report_error(err, *e);
      return;
  }
  
  // Check if comparison is valid
  if (!checker::check_operator_compatibility(lhs_type, rhs_type, op)) {
    std::string err = "Cannot compare " + lhs_type->get_name() + 
                      " and " + rhs_type->get_name() + " with operator " + op;
    report_error(err, *e);
    return;
  }
  
  // Result type is always boolean
  e->set_type(sym::Type::create_bool_type());
}

void SemanticAnalyser::visit(std::shared_ptr<expr::Variable>& e) {
  // Look up the variable in the symbol table
  std::string var_name = e->identifier()->name();
  auto symbol = symbol_table_->lookup_symbol(var_name);
  
  if (!symbol) {
    std::string err = "Undefined variable: " + var_name;
    report_error(err, *e);
    return;
  }
  
  // Check if it's a variable or parameter
  if (symbol->get_kind() != sym::SymbolKind::VAR && 
    symbol->get_kind() != sym::SymbolKind::PARAM) {
    std::string err = var_name + " is not a variable";
    report_error(err, *e);
    return;
  }
  
  // Mark as used
  symbol->set_used(true);
  
  // Set the type
  e->set_type(symbol->get_type());
}

void SemanticAnalyser::visit(std::shared_ptr<expr::Unary>& e) {
  visit(e->operand());
  
  auto op_type = e->operand()->type();
  
  // Check operation validity based on operator type
  switch (e->op()) {
    case expr::Unary::UnaryOp::NEG:
      // Negation requires numeric type
      if (!op_type->is_numeric_type()) {
        std::string err = "Unary negation requires numeric operand";
        report_error(err, *e);
        return;
      }
      // Result type is the same as operand type
      e->set_type(op_type);
      break;
        
    case expr::Unary::UnaryOp::NOT:
      // Bitwise NOT requires integral type
      if (!op_type->is_integral_type()) {
        std::string err = "Bitwise NOT requires integral operand";
        report_error(err, *e);
        return;
      }
      // Result type is the same as operand type
      e->set_type(op_type);
      break;
        
    case expr::Unary::UnaryOp::LOGICAL_NOT:
      // Result type is always boolean
      e->set_type(sym::Type::create_bool_type());
      break;
  }
}

void SemanticAnalyser::visit(std::shared_ptr<expr::Logical>& e) {
  visit(e->lhs());
  visit(e->rhs());
  
  // Both operands should be implicitly convertible to boolean
  auto lhs_type = e->lhs()->type();
  auto rhs_type = e->rhs()->type();
  
  bool lhs_valid = lhs_type->get_kind() == sym::Type::TypeKind::PRIMITIVE && 
                  lhs_type->get_name() == "bool";
  bool rhs_valid = rhs_type->get_kind() == sym::Type::TypeKind::PRIMITIVE && 
                  rhs_type->get_name() == "bool";
                  
  if (!lhs_valid) {
    std::string err = "Left operand of logical operator must be boolean";
    report_error(err, *e);
  }
  
  if (!rhs_valid) {
    std::string err = "Right operand of logical operator must be boolean";
    report_error(err, *e);
  }
  
  // Result type is boolean
  e->set_type(sym::Type::create_bool_type());
}

void SemanticAnalyser::visit(std::shared_ptr<expr::Constant>& e) {
  // Constants already have their type set during parsing
  // Nothing to do here for analysis
}

void SemanticAnalyser::visit(std::shared_ptr<expr::Bitwise>& e) {
  // Visit operands
  visit(e->lhs());
  visit(e->rhs());
  
  auto lhs_type = e->lhs()->type();
  auto rhs_type = e->rhs()->type();
  
  // Bitwise operations require integral types
  if (!lhs_type->is_integral_type()) {
    std::string err = "Left operand of bitwise operator must be an integral type";
    report_error(err, *e);
  }
  
  if (!rhs_type->is_integral_type()) {
    std::string err = "Right operand of bitwise operator must be an integral type";
    report_error(err, *e);
  }
  
  // Result type is typically the "larger" of the two types
  e->set_type(sym::Type::get_common_type(*lhs_type, *rhs_type));
}

void SemanticAnalyser::visit(std::shared_ptr<expr::boolean::Boolean>& e) {
  // Boolean literals always have boolean type
  e->set_type(sym::Type::create_bool_type());
}