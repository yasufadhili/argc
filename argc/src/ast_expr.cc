#include <iostream>
#include <utility>
#include <variant>

#include "include/ast.hh"
#include "include/sym_table.hh"
#include "include/util_logger.hh"

using namespace ast;
using namespace ast::expr;

Unary::Unary(const UnaryOp op, std::shared_ptr<Expression> operand)
  : op_(op), operand_(std::move(operand)) {}

void Unary::accept(SemanticAnalyser&) {

}

void Unary::accept(CodeGenerator&g) {

}

void Unary::print(const int level) {
  print_indent(level);
  std::cout << "Unary \n";
}

Binary::Binary(const std::variant<BinaryOp, RelationalOp> op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
  : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

void Binary::accept(SemanticAnalyser&an) {  
  an.visit(lhs());
  an.visit(rhs());
}

void Binary::accept(CodeGenerator&g) {

}

void Binary::print(const int level) {
  print_indent(level);
  std::cout << "Binary Expression " << "\n";
  if (lhs()) {
    lhs()->print(level + 1);
  }
  if (rhs()) {
    rhs()->print(level + 1);
  }
}

Literal::Literal(const LiteralVariant v)
  : value_(v){

}

void Literal::accept(SemanticAnalyser &an) {

}

void Literal::accept(CodeGenerator &g) {

}

void Literal::print(const int level) {
  print_indent(level);
  if (std::holds_alternative<int64_t>(value())) {
    std::cout << "Literal " << std::get<int64_t>(value()) << "\n";
  } 
  else if (std::holds_alternative<double>(value())) {
    std::cout << "Literal " << std::get<double>(value()) << "\n";
  }
}

Variable::Variable(std::shared_ptr<ident::Identifier> id, std::shared_ptr<sym_table::Type> t)
  : identifier_(std::move(id)), type_(std::move(t)) { }

void Variable::accept(SemanticAnalyser &an) {
  const std::string var_name { identifier()->name() };
  const auto symbol { an.symbol_table()->lookup_symbol(var_name) };

  if (!symbol) {
    an.add_error("reference to undefined variable: " + var_name, location());
    return;
  }

  // Check if it's a variable name or parameter
  if (
    symbol->get_kind()
    != sym_table::SymbolKind::VAR 
    && symbol->get_kind() 
    != sym_table::SymbolKind::PARAM
  ) {
    an.add_error(var_name + " is not a variable", location());
    return;
  }

  // Mark as used
  symbol->set_used(true);

  // Set the type
  set_type(symbol->get_type());
}

void Variable::accept(CodeGenerator &g) {

}

void Variable::print(const int level) {
  print_indent(level);
  std::cout << "Variable Expression " << "\n";
  identifier()->print(level + 1);
}