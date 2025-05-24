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
  public:
    std::vector<stmt_ptr> statements_;
  };

  class BinaryExpression final : public Expression {
  public:
    std::string op_;
    expr_ptr lhs_;
    expr_ptr rhs_;
  };

  using LiteralVariant = std::variant<int>;

  class LiteralExpression final : public Expression {
  public:
    LiteralVariant value_;
  };

  class ExpressionStatement final : public Statement {
  public:
    expr_ptr expression_;
  };

  class ReturnStatement final : public Statement {
  public:
    expr_ptr expression_;
  };

}

