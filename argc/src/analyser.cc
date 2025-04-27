#include "analyser.hh"

auto analyser::SemanticAnalyser::analyse(Node &node) -> bool {
  has_errors_ = false;
  node.accept(*this);
  return !has_errors_;
}
