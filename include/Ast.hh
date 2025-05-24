#pragma once

#include <memory>
#include <vector>
#include <string>
#include <variant>

namespace argc::ast {

  class Node {
  public:
    Node () = default;
    virtual ~Node() = default;
  };

  class Expression : public Node {};
  class Statement : public Node {};

  using node_ptr = std::unique_ptr<Node>;
  using expr_ptr = std::unique_ptr<Expression>;
  using stmt_ptr = std::unique_ptr<Statement>;

  class ModuleDeclaration final : public Node {
    std::vector<stmt_ptr> statements_;
  public:
    auto statements () -> std::vector<stmt_ptr>& { return statements_; }
  };

  class BinaryExpression final : public Expression {
    std::string op_;
    expr_ptr lhs_;
    expr_ptr rhs_;
  public:
    auto op () -> std::string& { return op_; }
    auto lhs () -> expr_ptr& { return lhs_; }
    auto rhs () -> expr_ptr& { return rhs_; }
  };

  using LiteralVariant = std::variant<int>;

  class LiteralExpression final : public Expression {
    LiteralVariant value_;
  public:
    auto value () -> LiteralVariant& { return value_; }
  };

  class ExpressionStatement final : public Statement {
    expr_ptr expression_;
  public:
    auto expression () -> expr_ptr& { return expression_; }
  };

  class ReturnStatement final : public Statement {
    expr_ptr expression_;
  public:
    auto expression () -> expr_ptr& { return expression_; }
  };

}

