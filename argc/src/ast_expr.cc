#include <iostream>
#include <utility>
#include <variant>

#include "ast.hh"

using namespace ast::expr;
using namespace ast::expr::arith;
using namespace ast::expr::rel;
using namespace ast::expr::boolean;


void Expression::print(const int level) {
  print_indent(level);
  std::cout << "Expression\n";
}

void Expression::accept(Visitor &) {

}

int Expression::evaluate() {

}


void Binary::print(const int level) {
  print_indent(level);
  std::cout << "Binary\n";
}

int Binary::evaluate() {

}

Unary::Unary(const UnaryOp op, std::shared_ptr<Expression> operand) : op(op), operand(std::move(operand)) {
}


void Unary::print(const int level) {
  print_indent(level);
  std::string op_str;
  switch (op) {
    case UnaryOp::NOT:
      op_str = "!";
      break;
    case UnaryOp::NEG:
      op_str =  "-";
      break;
    default:
      op_str =  "";
  }
  std::cout << "Unary " << op_str << "\n";
  operand->print(level + 1);
}

void Unary::accept(Visitor& v) {

}

int Unary::evaluate() {

}

Constant::Constant(const_variant v, sym::Type::TypeKind k) : value_(std::move(v)), kind_(k) {
}

void Constant::print(const int level) {
  print_indent(level);
  std::cout << "Constant: ";

  if (std::holds_alternative<int>(value_)) {
    std::cout << std::get<int>(value_) << " -> int" << '\n';
  } else if (std::holds_alternative<double>(value_)) {
    std::cout << std::get<double>(value_) << " -> double" << '\n';
  } else if (std::holds_alternative<std::string>(value_)) {
    std::cout << std::get<std::string>(value_) << " -> str" << '\n';
  } else if (std::holds_alternative<char>(value_)) {
    std::cout << std::get<char>(value_) << " -> char" << '\n';
  }
}

void Constant::accept(Visitor&v) {

}

auto Constant::evaluate() -> int {
  if (std::holds_alternative<int>(value_)) {
    return std::get<int>(value_);
  }
  return 0;
}

Variable::Variable(const std::string &n) {
}

void Variable::print(const int level) {
  print_indent(level);
  //std::cout << "Variable: " << identifier->get_name() << "\n";
  std::cout << "Variable: \n";
}

void Variable::accept(Visitor &v) {

}


Arithmetic::Arithmetic(const ArithmeticType t, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
  : type_(t), lhs_(std::move(l)), rhs_(std::move(r)) {
}


void Arithmetic::print(const int level) {
  print_indent(level);
  char op;
  switch (type_) {
    case ArithmeticType::ADD:
      op = '+';
      break;
    case ArithmeticType::SUB:
      op = '-';
      break;
    case ArithmeticType::MUL:
      op = '*';
      break;
    case ArithmeticType::DIV:
      op = '/';
      break;
    case ArithmeticType::MOD:
      op = '%';
      break;
    default:
      op = '.';
      break;
  }
  std::cout << "Arithmetic Expression: " << op << '\n';
  lhs_->print(level + 1);
  rhs_->print(level + 1);
}

void Arithmetic::accept(Visitor&v) {
  
}

int Arithmetic::evaluate() {
  switch (type_) {
    case ArithmeticType::ADD:
      return lhs_->evaluate() + rhs_->evaluate();
    case ArithmeticType::SUB:
      return lhs_->evaluate() - rhs_->evaluate();
    case ArithmeticType::MUL:
      return lhs_->evaluate() * rhs_->evaluate();
    case ArithmeticType::DIV:
      return lhs_->evaluate() / rhs_->evaluate();
    case ArithmeticType::MOD:
      return lhs_->evaluate() % rhs_->evaluate();
    default:
      return 0;
  }
  return 0;
}


Boolean::Boolean(const BooleanType t) : type(t) {
}

void Boolean::print(const int level) {
  print_indent(level);
  std::cout << "Boolean\n";
}

void Boolean::accept(Visitor &) {

}

int Boolean::evaluate() {

}

Relational::Relational(const RelationalType t, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
  : type_(t), lhs_(std::move(l)), rhs_(std::move(r)) {
}

void Relational::print(const int level) {
  print_indent(level);
  std::string op;
  switch (type_) {
    case RelationalType::EQ:
      op = "==";
      break;
    case RelationalType::NEQ:
      op = "!=";
      break;
    case RelationalType::LT:
      op = "<";
      break;
    case RelationalType::GT:
      op = ">";
      break;
    case RelationalType::GEQ:
      op = ">=";
      break;
    case RelationalType::LEQ:
      op = "<=";
      break;
    case RelationalType::NONE:
      op = "None";
      break;
    default:
      op = "Unknown";
      break;
  }
  std::cout << "Relational Expression: " << op << '\n';
  lhs_->print(level + 1);
  if (rhs_) {
    rhs_->print(level + 1);
  }
}

void Relational::accept(Visitor&v) {
  v.emit("");
}

int Relational::evaluate() {
  if (type_ == RelationalType::NONE) {
    return true;
  }
  switch (type_) {
    case RelationalType::EQ:
      return lhs_->evaluate() == rhs_->evaluate();
    case RelationalType::NEQ:
      return lhs_->evaluate() != rhs_->evaluate();
    case RelationalType::LT:
      return lhs_->evaluate() < rhs_->evaluate();
    case RelationalType::GT:
      return lhs_->evaluate() > rhs_->evaluate();
    case RelationalType::GEQ:
      return lhs_->evaluate() >= rhs_->evaluate();
    case RelationalType::LEQ:
      return lhs_->evaluate() <= rhs_->evaluate();
    case RelationalType::NONE:
      return 1;
    default:
      return 0;
  }
}


Bitwise::Bitwise(BitwiseOp op, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right)
  : op_(op), lhs_(std::move(left)), rhs_(std::move(right))
{

}

void Bitwise::print(int level) {
  print_indent(level);
  std::cout << "Bitwise Expression \n";
}

void Bitwise::accept(Visitor&v) {

}

int Bitwise::evaluate() {

}


Logical::Logical(LogicalOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
  : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {

}


void Logical::print(int level) {
  print_indent(level);
  std::cout << "Logical Expression \n";
}

void Logical::accept(Visitor &v) {

}

int Logical::evaluate() {

}
