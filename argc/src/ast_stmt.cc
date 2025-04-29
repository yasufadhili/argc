
#include "include/ast.hh"

using namespace ast;
using namespace ast::stmt;

Asm::Asm(std::string s)
  : asm_(std::move(s))
{

}

void Asm::accept(SemanticAnalyser &) {

}

void Asm::accept(x86_64_CodeGenerator &g) {
}

void Asm::print(const int level) {
  print_indent(level);
  std::cout << "Asm Statement \n";
}

