#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <sstream>


namespace ast {


  class Visitor;

  struct Node {
    virtual ~Node() = default;
    virtual void accept(Visitor& ) = 0;
  };

  namespace stmt {

    struct Statement : Node {
      void accept(Visitor& visitor) override;
      ~Statement() override = default;
    };

    struct Block : Statement {
      std::vector<std::shared_ptr<Statement>> statements;

      explicit Block(std::vector<std::shared_ptr<Statement>> statements) : statements(std::move(statements)) {}
      void accept(Visitor& visitor) override;
    };

    struct Return : Statement {
      std::optional<std::string> expr;

      void accept(Visitor& visitor) override;
      explicit Return(std::optional<std::string> expr) : expr(std::move(expr)) {}
    };

    struct Exit : Statement {
      std::string value;
      void accept(Visitor& visitor) override;

      explicit Exit(std::string value) : value(std::move(value)) {}
    };

  }

  namespace func {

    struct Function final : Node {
      std::string name;
      std::string return_type;
      std::vector<std::pair<std::string, std::string>> params;
      std::shared_ptr<Node> body;

      Function(std::string name,
        std::string return_type,
        std::vector<std::pair<std::string,
        std::string>> params,
        std::shared_ptr<Node> body)
        :
      name(std::move(name)),
      return_type(std::move(return_type)),
      params(std::move(params)),
      body(std::move(body))
      {}
      void accept(Visitor& ) override;
    };

  }


  namespace prog {

    struct Program final : Node {
      std::vector<std::shared_ptr<func::Function>> functions;
      void accept(Visitor& ) override;
      explicit Program(const std::vector<std::shared_ptr<func::Function>> &functions) : functions(functions) {}
      ~Program() override = default;
    };

  }



  class Visitor final {

    std::stringstream output_;

    auto emit(const std::string_view &code) -> void {
      output_ << code << "\n";
    }

    static std::string generate_label(const std::string &prefix) {
      static int counter = 0;
      return prefix + "_" + std::to_string(counter++);
    }

  public:

    Visitor() = default;

    ~Visitor() = default;

    auto get_output() const -> std::string {
      return output_.str();
    }

    void visit(prog::Program&);
    void visit(const func::Function&);
    void visit(stmt::Statement&);
    void visit(stmt::Block&);
    void visit(stmt::Return&);
    void visit(stmt::Exit&);

  };


  inline void prog::Program::accept(Visitor &v) {
    v.visit(*this);
  }

  inline void func::Function::accept(Visitor &v) {
    v.visit(*this);
  }

  inline void stmt::Statement::accept(Visitor &v) {
    v.visit(*this);
  }

  inline void stmt::Block::accept(Visitor &v) {
    v.visit(*this);
  }

  inline void stmt::Return::accept(Visitor &v) {
    v.visit(*this);
  }

  inline void stmt::Exit::accept(Visitor &v) {
    v.visit(*this);
  }




}


