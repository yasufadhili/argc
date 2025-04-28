#include "analyser.hh"
#include "ast.hh"
#include "symbols.hh"
#include <memory>

using namespace analyser;

SymbolCollector::SymbolCollector() {

}

/** TODO **/
SemanticAnalyser::SemanticAnalyser(){}

//SemanticAnalyser::SemanticAnalyser()
//  : symbol_table_(sym::SymbolTable::get_instance()),
//  error_occured_(false),
//  current_function_return_type(nullptr) {

//}

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
  
}