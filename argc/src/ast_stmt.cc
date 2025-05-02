
#include "include/ast.hh"
#include <memory>
#include <utility>

using namespace ast;
using namespace ast::stmt;

Block::Block(std::vector<std::shared_ptr<Statement>> stmts)
  : statements_(std::move(stmts))
{

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

void Return::print(const int level) {
  print_indent(level);
  std::cout << "Return Statement " <<'\n';
  if (expression().has_value()) {
    expression().value()->print(level + 1);
  }
}

VariableDeclaration::VariableDeclaration (
  std::shared_ptr<ident::Identifier> id,
  std::shared_ptr<sym_table::Type> t,
  std::optional<std::shared_ptr<expr::Expression>> init
) :
  identifier_(std::move(id)),
  type_(std::move(t)),
  initialiser_(std::move(init)) {}

void VariableDeclaration::print(const int level) {
  print_indent(level);
  std::cout << "Variable Declaration Statement " <<'\n';
  if (initialiser().has_value()) {
    initialiser().value()->print(level + 1);
  }
}

Assignment::Assignment (
  std::shared_ptr<ident::Identifier> target,
  std::shared_ptr<expr::Expression> val
) : target_(std::move(target)), value_(std::move(val)) {}

void Assignment::print(const int level) {
  print_indent(level);
  std::cout << "Assignment Statement \n";
  value()->print(level + 1);
}

Print::Print(std::shared_ptr<expr::Expression> e) : expression_(std::move(e)) {}

void Print::print(const int level) {
  print_indent(level);
  //int x = std::get<int>(expression()->evaluate());
  std::cout << "Print Statement \n";
  expression()->print(level + 1);
}