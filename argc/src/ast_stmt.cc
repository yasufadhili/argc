
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
  for (const auto &stmt : statements()) {
    stmt->print(level + 1);
  }
}

void Empty::print(const int level) {
  print_indent(level);
  std::cout << "Empty Statement " <<'\n';
}

Return::Return(std::optional<std::shared_ptr<expr::Expression> > expr)
  : expression_(std::move(expr)){

}

void Return::accept(SemanticAnalyser &) {

}

void Return::accept(CodeGenerator &) {

}

void Return::print(const int level) {
  print_indent(level);
  std::cout << "Return Statement " <<'\n';
  if (expression().has_value()) {
    expression().value()->print(level + 1);
  }
}

