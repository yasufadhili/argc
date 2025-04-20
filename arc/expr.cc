#include <iostream>

#include "include/ast.hh"


namespace ast::expr {


void Expression::print(int level) {
  Node::print_indent(level);
  std::cout << "Expression\n";
}

void Binary::print(int level) {
  Node::print_indent(level);
  std::cout << "Binary\n";
}

void Unary::print(int level) {
  Node::print_indent(level);
  std::cout << "Unary\n";
}


namespace arith {
  void Arithmetic::print(int level) {
    Node::print_indent(level);
    std::cout << "Arithmetic\n";
  }

  Add::Add(Arithmetic *lhs, Arithmetic *rhs) : lhs(lhs), rhs(rhs) {
  }

  Add::~Add() {
    delete lhs;
    delete rhs;
  }

  void Add::print(int level) {
    Node::print_indent(level);
    std::cout << "Add\n";
    if (lhs) lhs->print(level + 1);
    if (rhs) rhs->print(level + 1);
  }

  Sub::Sub(Arithmetic *lhs, Arithmetic *rhs) : lhs(lhs), rhs(rhs) {
  }

  Sub::~Sub() {
    delete lhs;
    delete rhs;
  }

  void Sub::print(int level) {
    Node::print_indent(level);
    std::cout << "Sub\n";
    if (lhs) lhs->print(level + 1);
    if (rhs) rhs->print(level + 1);
  }

  Mul::Mul(Arithmetic *lhs, Arithmetic *rhs) : lhs(lhs), rhs(rhs) {
  }

  Mul::~Mul() {
    delete lhs;
    delete rhs;
  }

  void Mul::print(int level) {
    Node::print_indent(level);
    std::cout << "Mul\n";
    if (lhs) lhs->print(level + 1);
    if (rhs) rhs->print(level + 1);
  }

  Div::Div(Arithmetic *lhs, Arithmetic *rhs) : lhs(lhs), rhs(rhs) {
  }

  Div::~Div() {
    delete lhs;
    delete rhs;
  }

  void Div::print(int level) {
    Node::print_indent(level);
    std::cout << "Div\n";
    if (lhs) lhs->print(level + 1);
    if (rhs) rhs->print(level + 1);
  }

  Mod::Mod(Arithmetic *lhs, Arithmetic *rhs) : lhs(lhs), rhs(rhs) {
  }

  Mod::~Mod() {
    delete lhs;
    delete rhs;
  }

  void Mod::print(int level) {
    Node::print_indent(level);
    std::cout << "Mod\n";
    if (lhs) lhs->print(level + 1);
    if (rhs) rhs->print(level + 1);
  }

  void Neg::print(int level) {
    Node::print_indent(level);
    std::cout << "Neg\n";
  }

  void Not::print(int level) {
    Node::print_indent(level);
    std::cout << "Not\n";
  }
}

namespace boolean {
  void Boolean::print(int level) {
    Node::print_indent(level);
    std::cout << "Boolean\n";
  }

  void True::print(int level) {
    Node::print_indent(level);
    std::cout << "True\n";
  }

  void False::print(int level) {
    Node::print_indent(level);
    std::cout << "False\n";
  }
}

namespace rel {
  void Relational::print(int level) {
    Node::print_indent(level);
    std::cout << "Relational\n";
  }

  void Equal::print(int level) {
    Node::print_indent(level);
    std::cout << "Equal\n";
  }

  void NotEqual::print(int level) {
    Node::print_indent(level);
    std::cout << "NotEqual\n";
  }

  void GreaterThan::print(int level) {
    Node::print_indent(level);
    std::cout << "GreaterThan\n";
  }

  void GreaterThanOrEqual::print(int level) {
    Node::print_indent(level);
    std::cout << "GreaterThanOrEqual\n";
  }

  void LessThan::print(int level) {
    Node::print_indent(level);
    std::cout << "LessThan\n";
  }

  void LessThanOrEqual::print(int level) {
    Node::print_indent(level);
    std::cout << "LessThanOrEqual\n";
  }
}
}

