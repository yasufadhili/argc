#include <iostream>
#include <utility>

#include "include/ast.hh"

namespace ast::stmt {

  void Statement::print(const int level) {
    Node::print_indent(level);
    std::cout << "Statement\n";
  }

  Block::Block(std::vector<Statement *> stmts) : statements(std::move(stmts)) {

  }

  Block::~Block() {
    for (const auto s : statements) {
      delete s;
    }
  }


  void Block::print(const int level) {
    Node::print_indent(level);
    std::cout << "Block\n";
    for (const auto stmt : statements) {
      if (stmt) stmt->print(level + 1);
    }
  }

  Initialise::Initialise(expr::Expression *) : expression(nullptr) {
  }

  void Initialise::print(const int level) {
    Node::print_indent(level);
    std::cout << "Initialise\n";
  }


  Assign::Assign(expr::Expression *) : expression(nullptr) {

  }

  Assign::~Assign() {
    delete expression;
  }

  void Assign::print(const int level) {
    Node::print_indent(level);
    std::cout << "Assign\n";
  }

  VariableDeclaration::VariableDeclaration(
    const std::string& name,
    const std::shared_ptr<sym::Type> &type,
    const std::shared_ptr<expr::Expression> &initialiser = nullptr
    )
      :
  name(name),
  type(type),
  initialiser(initialiser),
  symbol(nullptr)
  {

  }

  auto VariableDeclaration::get_name() const -> std::string {
    return name;
  }

  auto VariableDeclaration::get_initialiser() const -> std::shared_ptr<expr::Expression> {
    return initialiser;
  }

  auto VariableDeclaration::get_type() const -> std::shared_ptr<sym::Type> {
    return type;
  }

  auto VariableDeclaration::set_symbol(std::shared_ptr<sym::Symbol> sym) -> void {
    symbol = std::move(sym);
  }

  auto VariableDeclaration::get_symbol() const -> std::shared_ptr<sym::Symbol> {
    return symbol;
  }

  RegisterAssign::RegisterAssign(expr::Expression *exp) : expression(exp) {
  }

  RegisterAssign::~RegisterAssign() {
    delete expression;
  }


  void RegisterAssign::print(const int level) {
    Node::print_indent(level);
    std::cout << "RegisterAssign\n";
  }



}