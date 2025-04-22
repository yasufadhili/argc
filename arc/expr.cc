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

  Constant::Constant(const const_variant v, sym::Type::TypeKind k) : value(v), kind(k) {
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

  Variable::Variable(const std::string &n) {
  }

  void Variable::print(const int level = 0) {
    Node::print_indent(level);
    //std::cout << "Variable: " << identifier->get_name() << "\n";
    std::cout << "Variable: \n";
  }

  namespace arith {
    Arithmetic::Arithmetic(const ArithmeticType t, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
      : type(t), lhs(std::move(l)), rhs(std::move(r)) {
    }


    void Arithmetic::print(const int level) {
      Node::print_indent(level);
      char op;
      switch (type) {
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
      lhs->print(level + 1);
      rhs->print(level + 1);
    }
  } // namespace arith

  namespace boolean {
    Boolean::Boolean(const BooleanType t) : type(t) {
    }

    void Boolean::print(const int level) {
      Node::print_indent(level);
      std::cout << "Boolean\n";
    }
  } // namespace boolean

  namespace rel {
    Relational::Relational(const RelationalType t, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r)
      : type(t), lhs(std::move(l)), rhs(std::move(r)) {
    }

    void Relational::print(const int level) {
      Node::print_indent(level);
      std::string op;
      switch (type) {
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
        default:
          op = "Unknown";
          break;
      }
      std::cout << "Relational Expression: " << op << '\n';
      lhs->print(level + 1);
      rhs->print(level + 1);
    }
  }
}
