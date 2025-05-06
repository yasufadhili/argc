#pragma once

#include "ast.hh"

namespace ast {

class SemanticAnalyser final : public Visitor {
  std::shared_ptr<sym_table::SymbolTable> symbol_table_;
  bool error_occurred_;
  std::shared_ptr<sym_table::Type> current_return_type_;
  config::Config config_;
public:
  SemanticAnalyser() : symbol_table_(sym_table::SymbolTable::get_instance()), error_occurred_(false) {}
  ~SemanticAnalyser() override = default;

  bool has_errors() const { return error_occurred_; }
  
  void visit(mod::Module&) override;
  void visit(func::Function&) override;
  void visit(expr::FunctionCall&) override;
  void visit(func::Parameter&) override;
  void visit(func::SingleReturnType&) override;
  void visit(func::MultipleReturnType&) override;
  void visit(func::Body&) override;
  void visit(func::ReturnTypeInfo&) override;
  void visit(ident::Identifier&) override;
  void visit(ident::TypeIdentifier&) override;
  void visit(stmt::Statement&) override;
  void visit(stmt::Empty&) override;
  void visit(stmt::Block&) override;
  void visit(stmt::Return&) override;
  void visit(stmt::Print&) override;
  void visit(stmt::VariableDeclaration&) override;
  void visit(stmt::Assignment&) override;
  void visit(expr::Expression&) override;
  void visit(expr::Literal&) override;
  void visit(expr::Binary&) override;
  void visit(expr::Unary&) override;
  void visit(expr::Variable&) override;

};

}
  