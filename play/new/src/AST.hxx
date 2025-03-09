#pragma once

#include <utility>


namespace ast {


  struct Node {
    virtual ~Node() = default;
  };

  namespace expr {

    struct Expr : Node {};

    struct Constant final : Expr {
      std::string value;
      ~Constant() override = default;
      explicit Constant(std::string v) : value(std::move(v)) {}
    };

  }


  namespace stmt {

    struct Stmt : Node {
      std::unique_ptr<expr::Expr> expr;

      explicit Stmt(std::unique_ptr<expr::Expr> expr) : expr(std::move(expr)) {}
    };

    struct Exit final : Stmt {
      std::unique_ptr<expr::Expr> expr;
      explicit Exit(const std::string& value) : Stmt(std::make_unique<expr::Constant>(value)) {}
    };

  }


}

