#pragma once

#include <memory>
#include <vector>
#include <string>
#include <variant>

#include "antlr4-runtime.h"

namespace argc::ast {

  class Node {
  public:
    Node () = default;
    virtual ~Node() = default;
  };

  class Expression : public Node {};
  class Statement : public Node {};

  using node_ptr = std::shared_ptr<Node>;
  using expr_ptr = std::shared_ptr<Expression>;
  using stmt_ptr = std::shared_ptr<Statement>;

  class ModuleDeclaration final : public Node {
    std::vector<stmt_ptr> statements_;
  public:
    auto statements () -> std::vector<stmt_ptr>& { return statements_; }
  };

  class BinaryExpression final : public Expression {
    std::string op_;
    expr_ptr lhs_;
    expr_ptr rhs_;
  };

  using LiteralVariant = std::variant<int>;

  class LiteralExpression final : public Expression {
    LiteralVariant value_;
  };

  class ExpressionStatement final : public Statement {
    expr_ptr expression_;
  };

  class ReturnStatement final : public Statement {
    expr_ptr expression_;
  public:
    auto expression () -> expr_ptr& { return expression_; }
  };

}

