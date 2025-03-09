#pragma once

#include <string>
#include <utility>
#include <sstream>
#include <vector>


namespace ast {

  class  Visitor;

  struct Node {
    virtual ~Node() = default;
    virtual void accept(Visitor& visitor) = 0;
  };

  namespace expr {

    struct Expr : Node {
      ~Expr() override = default;
      void accept(Visitor& visitor) override = 0;
    };

    struct Literal : Expr {
      std::string value;
      
      explicit Literal(std::string v) : value(std::move(v)) {}
      ~Literal() override = default;
    };

  }


  namespace stmt {

    struct Stmt : Node {
      ~Stmt() override = default;
    };

    struct Exit : Stmt {
      std::string value;
      void accept(Visitor& visitor) override;
      explicit Exit(std::string value) : value(std::move(value)) {}
      ~Exit() override = default;
    };

  }


  namespace func {

    struct Function : Node {
      std::string name;
      std::string return_type;
      explicit Function(
        std::string name,
        std::string return_type
        ) :
      name(std::move(name)),
      return_type(std::move(return_type))
      {}
      ~Function() override = default;
    };

  }


  namespace prog {
    struct Program : Node {
      std::vector<func::Function> functions;
      explicit Program(std::vector<func::Function> functions) : functions(std::move(functions)) {}
      ~Program() override = default;
    };
  }



  class Visitor {
    std::stringstream output_;

    auto emit(const std::string_view &code) -> void {
      output_ << code << "\n";
    }

    static auto generate_label(const std::string &prefix) -> std::string {
      static int counter = 0;
      return prefix + "_" + std::to_string(counter++);
    }

  public:
    Visitor();

    auto visit(const stmt::Exit &exit) -> void;

    auto get_output() const -> std::string {
      return output_.str();
    }

  };

  inline void stmt::Exit::accept(Visitor &visitor) {
    visitor.visit(*this);
  }


}



