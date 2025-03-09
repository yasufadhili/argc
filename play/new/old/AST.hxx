#pragma once
#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../src/TokenType.hxx"


namespace ast {


  struct Node {
    virtual ~Node() = default;
  };


  namespace expr {

    struct Expr : Node {};

    struct Unary final : Expr {
      TokenType op;
      Expr *operand;

      Unary(const TokenType op, Expr *operand) : op(op), operand(operand) {}
    };

    struct Binary final : Expr {
      Expr *left;
      TokenType op;
      Expr *right;

      Binary(Expr *left, const TokenType op, Expr *right) : left(left), op(op), right(right) {}
    };

    struct Ternary final : Expr {
      Expr *condition;
      TokenType op_left;
      Expr *true_value;
      TokenType op_right;
      Expr *false_value;

      Ternary(Expr *condition, const TokenType op_l, Expr *true_value, const TokenType op_r, Expr *false_value) :
      condition(condition), op_left(op_l), true_value(true_value), op_right(op_r), false_value(false_value)
      {}
    };

    struct Factor final : Expr {};

    struct Constant final : Expr {
      std::string name;
      std::variant<int> value;
    };

    //struct Assign : Expr {};
    //struct Add : Expr {};

  }


  namespace stmt {

    struct Stmt : Node {};

    struct Exit final : Stmt {};

    struct Return final : Stmt {};

    struct Let final : Stmt {};

  }

  namespace block {

    struct Block final : Node {
      std::vector<stmt::Stmt*> stmts;
      auto add_stmt(stmt::Stmt *stmt) -> void {
        stmts.push_back(stmt);
      }
      explicit Block(std::vector<stmt::Stmt*> stmts) : stmts(std::move(stmts)) {}
    };

  }


  namespace scope {

    struct Scope final : Node {
      std::vector<Node*> nodes;
      auto add_node(Node *node) -> void {
        nodes.push_back(node);
      }
      explicit Scope(std::vector<Node*> nodes) : nodes(std::move(nodes)) {}
    };

  }


  namespace func {

    struct Func final : Node {
      std::string name;
      std::string return_type;
      std::vector<std::string, std::string> params;
      std::vector<stmt::Stmt*> stmts;
      std::map<std::string, std::string> locals;
    };

  }



}


