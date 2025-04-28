#include <iostream>
#include <memory>
#include <string>
#include <utility>
 
#include "ast.hh"

using namespace ast::stmt;

void Statement::print(const int level) {
  Node::print_indent(level);
  std::cout << "Statement \n";
}

void Statement::accept(Visitor &v) {
}


void Block::print(const int level) {
  Node::print_indent(level);
  std::cout << "Block Statement \n";
  for (const auto &stmt: statements_) {
    if (stmt) stmt->print(level + 1);
  }
}

void Block::accept(Visitor& v) {
  for (const auto& s: statements_) {
    s->accept(v);
  }
}

ExpressionStatement::ExpressionStatement(std::shared_ptr<expr::Expression> expr) : expression(std::move(expr)) {
}

void ExpressionStatement::print(const int level) {
  Node::print_indent(level);
  std::cout << "Expression Statement\n";
  expression->print(level + 1);
}

void ExpressionStatement::accept(Visitor& v) {

}

VariableDeclaration::VariableDeclaration(
  std::string name,
  const std::shared_ptr<sym::Type> &type,
  const std::optional<std::shared_ptr<expr::Expression> > &init = std::nullopt
)
  : name_(std::move(name)),
    type_(type),
    initialiser_(init),
    symbol_(nullptr) {
}

void VariableDeclaration::print(const int level) {
  Node::print_indent(level);
  std::cout << "Variable Declaration " << name_ << " -> " << "\n";
  if (initialiser_.has_value()) {
    initialiser_.value()->print(level + 1);
  }
}

void VariableDeclaration::accept(Visitor &) {

}

Assignment::Assignment(std::string target_var, std::shared_ptr<expr::Expression> assigned_value)
  : target_(std::move(target_var)),
    value_(std::move(assigned_value)) {
}

void Assignment::print(const int level) {
  Node::print_indent(level);
  std::cout << "Assignment " << target_ << "\n";
  value_->print(level + 1);
}

void Assignment::accept(Visitor& v) {

}

Return::Return(std::optional<std::shared_ptr<expr::Expression> > expr)
  : expression_(std::move(expr))
{

}

void Return::print(const int level) {
  print_indent(level);
  std::cout << "Return Statement\n";
  if (expression_.has_value()) {
    expression_.value()->print(level + 1);
  }
}

void Return::accept(Visitor& v) {
  if (!expression_.has_value()) {
    v.emit("  ret");
  }
  v.emit("  mov $" + std::to_string( expression_.value()->evaluate()) + ", %rax");
  v.emit("  ret");
}

Repeat::Repeat(std::optional<std::shared_ptr<expr::Expression> > times) : times_(std::move(times)) {

}

void Repeat::print(const int level) {
  print_indent(level);
  std::cout << "Repeat Statement \n";
  if (times_.has_value()) {
    times_.value()->print(level + 1);
  } else {
    print_indent(level);
    std::cout << "Infinite \n";
  }
}

void Repeat::accept(Visitor &) {

}


void EmptyStatement::print(const int level) {
  Node::print_indent(level);
  std::cout << "Empty Statement\n";
}

void EmptyStatement::accept(Visitor &) {

}

