
#include "include/ast.hh"

using namespace ast;
using namespace ast::stmt;

Block::Block(std::vector<std::shared_ptr<Statement>> stmts)
  : statements_(std::move(stmts))
{

}

void Block::accept(SemanticAnalyser &) {

}

void Block::accept(CodeGenerator &) {

}

void Block::print(const int level) {
  print_indent(level);
  std::cout << "Block Statement " <<'\n';
}

