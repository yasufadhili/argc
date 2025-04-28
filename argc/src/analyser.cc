#include "analyser.hh"
#include "ast.hh"
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