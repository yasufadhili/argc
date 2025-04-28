#include "analyser.hh"
#include "symbols.hh"

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
