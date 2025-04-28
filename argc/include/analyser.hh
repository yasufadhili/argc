#pragma once
#include "ast.hh"
#include "symbols.hh"
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
  auto report_error(std::string& message, const Node& node) -> void { /** TODO */ };
  auto report_warning(std::string& message, const Node& node) -> void { /** TODO  */};
public:
  void visit(std::shared_ptr<prog::Program>&) override = 0;
  void visit(std::shared_ptr<prog::Module>&) override = 0;
  void visit(std::shared_ptr<func::Function>&) override = 0;
  void visit(std::shared_ptr<stmt::Return>&) override = 0;
  
};

}
