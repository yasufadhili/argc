#pragma once

#include <vector>
#include <iostream>

#include "symbols.hh"

namespace ast {

  class Node {
  protected:
    static void print_indent(const int level) {
      for (int i = 0; i < level; ++i) {
        std::cout << "  ";
      }
    }
  public:
    virtual ~Node() = default;
    virtual void print(int level = 0) = 0;
  };

  namespace expr {

    class Expression : public Node {
    public:
      ~Expression() override = default;
      void print(int level = 0) override;
    };

    class Binary : public Expression {
    public:
      ~Binary() override = default;
      void print(int level = 0) override;
    };

    class Unary : public Expression {
    public:
      ~Unary() override = default;
      void print(int level = 0) override;
    };

    class Constant final : public Expression {
      std::string value;
    public:
      explicit Constant(std::string val);
      ~Constant() override = default;
      void print(int level = 0) override;
    };

    namespace arith {

      class Arithmetic : public Binary {
      public:
        ~Arithmetic() override = default;
        void print(int level = 0) override;
      };

      class Add final : public Arithmetic {
        Expression* lhs;
        Expression* rhs;
      public:
        Add(Expression* lhs, Expression* rhs);
        ~Add() override;
        void print(int level = 0) override;
      };

      class Sub final : public Arithmetic {
        Arithmetic* lhs;
        Arithmetic* rhs;
      public:
        Sub(Arithmetic* lhs, Arithmetic* rhs);
        ~Sub() override;
        void print(int level = 0) override;
      };

      class Mul final : public Arithmetic {
        Arithmetic* lhs;
        Arithmetic* rhs;
      public:
        Mul(Arithmetic* lhs, Arithmetic* rhs);
        ~Mul() override;
        void print(int level = 0) override;
      };

      class Div final : public Arithmetic {
        Arithmetic* lhs;
        Arithmetic* rhs;
      public:
        Div(Arithmetic* lhs, Arithmetic* rhs);
        ~Div() override;
        void print(int level = 0) override;
      };

      class Mod final : public Arithmetic {
        Arithmetic* lhs;
        Arithmetic* rhs;
      public:
        Mod(Arithmetic* lhs, Arithmetic* rhs);
        ~Mod() override;
        void print(int level = 0) override;
      };

      class Neg final : public Unary {
      public:
        ~Neg() override = default;
        void print(int level = 0) override;
      };

      class Not final : public Unary {
      public:
        ~Not() override = default;
        void print(int level = 0) override;
      };

    }


    namespace boolean {

      class Boolean : public Unary {
      public:
        ~Boolean() override = default;
        void print(int level = 0) override;
      };

      class True final: public Boolean {
      public:
        ~True() override = default;
        void print(int level = 0) override;
      };

      class False final: public Boolean {
      public:
        ~False() override = default;
        void print(int level = 0) override;
      };

    }


    namespace rel {

      class Relational : public Binary {
      public:
        ~Relational() override = default;
        void print(int level = 0) override;
      };

      class Equal final : public Relational {
      public:
        ~Equal() override = default;
        void print(int level = 0) override;
      };

      class NotEqual final : public Relational {
      public:
        ~NotEqual() override = default;
        void print(int level = 0) override;
      };

      class GreaterThan final : public Relational {
      public:
        ~GreaterThan() override = default;
        void print(int level = 0) override;
      };

      class LessThan final : public Relational {
      public:
        ~LessThan() override = default;
        void print(int level = 0) override;
      };

      class GreaterThanOrEqual final : public Relational {
      public:
        ~GreaterThanOrEqual() override = default;
        void print(int level = 0) override;
      };

      class LessThanOrEqual final : public Relational {
      public:
        ~LessThanOrEqual() override = default;
        void print(int level = 0) override;
      };

    }

  }


  namespace stmt {

    class Statement : public Node {
    public:
      ~Statement() override = default;
      void print(int level = 0) override;
    };

    class Block final: public Statement {
      std::vector<Statement*> statements;
    public:
      explicit Block(std::vector<Statement*>);

      ~Block() override;
      void print(int level = 0) override;
    };

    class Initialise final : public Statement {
      expr::Expression* expression;
    public:
      explicit Initialise(expr::Expression*);
      ~Initialise() override = default;
      void print(int level = 0) override;
    };

    class VariableDeclaration final : public Statement {
      std::string name;
      std::shared_ptr<sym::Type> type;
      std::shared_ptr<expr::Expression> initialiser;
      std::shared_ptr<sym::Symbol> symbol;
    public:
      VariableDeclaration(const std::string&name, const std::shared_ptr<sym::Type> &type, const std::shared_ptr<expr::Expression> &initialiser);

      auto get_name() const -> std::string;
      auto get_type() const -> std::shared_ptr<sym::Type>;
      auto get_initialiser() const -> std::shared_ptr<expr::Expression>;

      auto set_symbol(std::shared_ptr<sym::Symbol> sym) -> void;
      auto get_symbol() const -> std::shared_ptr<sym::Symbol>;

    };

    class Assign final : public Statement {
      expr::Expression* expression;
    public:
      explicit Assign(expr::Expression*);
      ~Assign() override;
      void print(int level = 0) override;
    };

    class RegisterAssign final : public Statement {
      std::string reg_name;
      expr::Expression* expression;
    public:
      explicit RegisterAssign(std::string, expr::Expression* exp);
      ~RegisterAssign() override;
      void print(int level = 0) override;
    };

  }


  namespace func {

    class Function final : public Node {
      stmt::Block *body;
    public:
      explicit Function(stmt::Block *b);
      ~Function() override;
      void print(int level = 0) override;
    };

  }


  namespace module {
    class Module final: public Node {
      std::vector<stmt::Statement *> statements;
      std::vector<func::Function *> functions;
    public:
      explicit Module(std::vector<stmt::Statement *> stmts, std::vector<func::Function *> fns);
      ~Module() override;
      void print(int level = 0) override;
    };
  }


  namespace prog {

    class Program final: public Node {
      std::vector<module::Module*> modules;
    public:
      explicit Program(std::vector<module::Module*> modules);
      ~Program() override;
      void print(int level = 0) override;
    };

  }


}
