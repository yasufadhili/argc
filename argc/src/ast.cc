#include "ast.hh"
#include <sstream>
#include <string>

using namespace ast;

auto Visitor::emit(std::string code) -> void {
  output_ << code << "\n";
}

auto Visitor::get_output() -> std::stringstream& {
  return output_;
}

auto Visitor::generate_label(int v) -> std::string {
  return std::to_string(v);
}