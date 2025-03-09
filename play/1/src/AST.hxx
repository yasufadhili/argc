#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <variant>
#include <sstream>

#include "Visitor.hxx"

using String = std::string;

namespace ast {

  class Visitor;

  struct Node {
    virtual ~Node() = default;

    virtual void accept(Visitor &visitor) = 0;
  };

  namespace expr {

    struct Expr : Node {
      ~Expr() override = default;

      auto accept(Visitor &visitor) -> void override = 0;
    };

    struct IntLit : Expr {
      int value;

      explicit IntLit(const int value) : value(value) {
      }

      auto accept(Visitor &visitor) -> void override;
    };

  }

  namespace stmt {
    struct Stmt : Node {
      ~Stmt() override = default;

      auto accept(Visitor &visitor) -> void override = 0;
    };

    struct Return : Stmt {
      std::optional<std::shared_ptr<expr::Expr>> expr;

      auto accept(Visitor &visitor) -> void override;

      explicit Return(std::optional<std::shared_ptr<expr::Expr>> expr) : expr(std::move(expr)) {
      }
    };
  }

  namespace func {
    struct Func : Node {

      String name;
      std::vector<std::shared_ptr<stmt::Stmt> > stmts;

      ~Func() override = default;

      auto accept(Visitor &visitor) -> void override;

      //Func(String& name, std::vector<expr::Expr>&& params, std::vector<stmt::Stmt>&& body) {}
      Func(String &name, std::vector<std::shared_ptr<stmt::Stmt>> stmts) : name(name), stmts(std::move(stmts)) {
      }
    };
  }

  namespace unit {
    struct Unit : Node {
      std::vector<std::shared_ptr<func::Func> > functions;

      auto accept(Visitor &visitor) -> void override;

      explicit Unit(std::vector<std::shared_ptr<func::Func> > functions) : functions(std::move(functions)) {
      }
    };
  }

  namespace prog {
    struct Program : Node {
      std::vector<std::shared_ptr<unit::Unit> > units;

      auto accept(Visitor &visitor) -> void override;

      explicit Program(std::vector<std::shared_ptr<unit::Unit> > units) : units(std::move(units)) {
      }
    };
  }



  class Visitor {

    std::stringstream output;

    auto emit(const std::string &code) -> void{
      output << code << "\n";
    }

    static auto generate_label(const std::string &prefix) -> std::string {
      static int counter = 0;
      return prefix + "_" + std::to_string(counter++);
    }

  public:

    auto visit(const prog::Program&) -> void;
    auto visit(const unit::Unit&) -> void;
    auto visit(const func::Func&) -> void;
    auto visit(const stmt::Return&) -> void;
    auto visit(const expr::IntLit&) -> void;

    auto get_output() const -> std::string { return output.str(); }

  };

  inline auto prog::Program::accept(Visitor &visitor) -> void {
    visitor.visit(*this);
  }

  inline auto unit::Unit::accept(Visitor &visitor) -> void {
    visitor.visit(*this);
  }

  inline auto func::Func::accept(Visitor &visitor) -> void {
    visitor.visit(*this);
  }

  inline auto stmt::Return::accept(Visitor &visitor) -> void {
    visitor.visit(*this);
  }

  inline auto expr::IntLit::accept(Visitor &visitor) -> void {
    visitor.visit(*this);
  }








}
