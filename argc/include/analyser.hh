#pragma once
#include "ast.hh"
#include "symbols.hh"
#include "util_logger.hh"
#include <memory>

using namespace ast;

namespace analyser {

class SymbolCollector : public Visitor {
  std::shared_ptr<sym::SymbolTable> symbol_table_;
  bool error_occured_;
  bool has_errors_;

public:
  SymbolCollector();
  auto has_errors() const -> bool { return error_occured_; }
};

class SemanticAnalyser final : public Visitor {
  std::shared_ptr<sym::SymbolTable> symbol_table_;
  bool error_occured_;
  std::shared_ptr<sym::Type> current_function_return_type;
public:
  SemanticAnalyser();
  auto analyse(Node& node) -> bool;
private:
  auto report_error(std::string& message, const Node& node) -> void { 
    LOG_ERROR(message);
    error_occured_ = true;
   };
  auto report_warning(std::string& message, const Node& node) -> void {
    LOG_WARNING(message);
  };
public:
  // Program structure
  void visit(std::shared_ptr<prog::Program>&) override;
  void visit(std::shared_ptr<prog::Module>&) override;
  void visit(std::shared_ptr<func::Function>&) override;

  // Statements
  void visit(std::shared_ptr<stmt::Statement>&) override;
  void visit(std::shared_ptr<stmt::Block>&) override;
  void visit(std::shared_ptr<stmt::Return>&) override;
  void visit(std::shared_ptr<stmt::VariableDeclaration>&) override;
  void visit(std::shared_ptr<stmt::Assignment>&) override;
  void visit(std::shared_ptr<stmt::ExpressionStatement>&) override;
  void visit(std::shared_ptr<stmt::Repeat>&) override;
  void visit(std::shared_ptr<stmt::EmptyStatement>&) override;

  // Expressions
  void visit(std::shared_ptr<expr::Expression>&) override;
  void visit(std::shared_ptr<expr::Binary>&) override;
  void visit(std::shared_ptr<expr::arith::Arithmetic>&) override;
  void visit(std::shared_ptr<expr::rel::Relational>&) override;
  void visit(std::shared_ptr<expr::Variable>&) override;
  void visit(std::shared_ptr<expr::Unary>&) override;
  void visit(std::shared_ptr<expr::Logical>&) override;
  void visit(std::shared_ptr<expr::Constant>&) override;
  void visit(std::shared_ptr<expr::Bitwise>&) override;
  void visit(std::shared_ptr<expr::boolean::Boolean>&) override;
  
};

}
