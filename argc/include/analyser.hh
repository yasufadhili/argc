#pragma once

#include "ast.hh"

using namespace ast;

namespace analyser {

class SemanticAnalyser final : public Visitor {
  std::shared_ptr<sym::SymbolTable> symbol_table_;
  bool has_errors_;
public:
  SemanticAnalyser();
  auto analyse(Node& node) -> bool;
private:
  auto report_error(std::string& message, const Node& node) -> void;
  auto report_warning(std::string& message, const Node& node) -> void;
};

}
