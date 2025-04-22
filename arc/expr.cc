#include <iostream>
#include <utility>

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

Unary::Unary(const UnaryOp op, std::shared_ptr<Expression> operand) : op(op), operand(std::move(operand)) {

}


void Unary::print(const int level) {
  Node::print_indent(level);
  std::cout << "Unary\n";
}

Constant::Constant(const const_variant v) : value(v) {

}

void Constant::print(const int level) {
  print_indent(level);
  std::cout << "Constant: ";
  if (std::holds_alternative<int>(value)) {
    std::cout << std::get<int>(value) << '\n';
  } else if (std::holds_alternative<double>(value)) {
    std::cout << std::get<double>(value) << '\n';
  }
}



namespace arith {

Arithmetic::Arithmetic(const ArithmeticType t, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
  : type(t), lhs(std::move(l)), rhs(std::move(r))
{

}


void Arithmetic::print(const int level) {
  Node::print_indent(level);
  std::cout << "Arithmetic\n";
  lhs->print(level + 1);
  rhs->print(level + 1);
}


}// namesapce arith

namespace boolean {

  Boolean::Boolean(const BooleanType t) : type(t) {

  }

  void Boolean::print(const int level) {
    Node::print_indent(level);
    std::cout << "Boolean\n";
  }

}// namespace boolean

namespace rel {
  void Relational::print(int level) {
    Node::print_indent(level);
    std::cout << "Relational\n";
  }

}

}

