#include <utility>

#include "include/ast.hh"

using namespace ast;
using namespace ast::expr;

Unary::Unary(const UnaryOp op, std::shared_ptr<Expression> operand)
  : op_(op), operand_(std::move(operand)) {}

void Unary::accept(SemanticAnalyser&) {

}

void Unary::accept(CodeGenerator&) {

}

void Unary::print(const int level) {
  print_indent(level);
  std::cout << "Unary \n";
}

Binary::Binary(const std::variant<BinaryOp, RelationalOp> op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
  : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

void Binary::accept(SemanticAnalyser&) {

}

void Binary::accept(CodeGenerator&) {

}

void Binary::print(const int level) {
  print_indent(level);
  std::cout << "Binary Expression " << "\n";
  lhs()->print(level + 1);
  rhs()->print(level + 1);
}

Literal::Literal(const int v)
  : value_(v){

}

void Literal::accept(SemanticAnalyser &) {

}

void Literal::accept(CodeGenerator &) {

}

void Literal::print(const int level) {
  print_indent(level);
  std::cout << "Literal " << value() << "\n";
}

