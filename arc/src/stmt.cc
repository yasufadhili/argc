#include <iostream>
#include <utility>

#include "include/ast.hh"

using namespace ast::stmt;

  void Statement::print(const int level) {
    Node::print_indent(level);
    std::cout << "Statement \n";
  }

  void Block::print(const int level) {
    Node::print_indent(level);
    std::cout << "Block Statement \n";
    for (const auto& stmt : statements) {
      if (stmt) stmt->print(level + 1);
    }
  }

  ExpressionStatement::ExpressionStatement(std::shared_ptr<expr::Expression> expr) : expression(std::move(expr)) {}

  void ExpressionStatement::print(const int level) {
    Node::print_indent(level);
    std::cout << "Expression Statement\n";
    expression->print(level +1);
  }

  VariableDeclaration::VariableDeclaration(
    std::string  name,
    const std::shared_ptr<sym::Type> &type,
    const std::optional<std::shared_ptr<expr::Expression>>& init = std::nullopt
  )
      :
  name(std::move(name)),
  type(type),
  initialiser(init),
  symbol(nullptr)
  {

  }

  void VariableDeclaration::print(const int level) {
    Node::print_indent(level);
    std::cout << "Variable Declaration " << name << " -> " << "\n";
    if (initialiser.has_value()) {
      initialiser.value()->print(level + 1);
    }
  }


  auto VariableDeclaration::get_name() const -> std::string {
    return name;
  }

  Assignment::Assignment(std::string target_var, std::shared_ptr<expr::Expression> assigned_value)
          : target(std::move(target_var)),
            value(std::move(assigned_value)) {}

  void Assignment::print(const int level) {
    Node::print_indent(level);
    std::cout << "Assignment " << target << "\n";
    value->print(level + 1);
  }

