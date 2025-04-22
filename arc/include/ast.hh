#pragma once

#include <memory>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>

#include "symbols.hh"

namespace ast {

  class Node {
  protected:
    static void print_indent(int level) {
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

    class Unary final : public Expression {
    public:
      enum struct UnaryOp {
        NEG, NOT
      };
    private:
      UnaryOp op;
      std::shared_ptr<Expression> operand;

    public:
      Unary(UnaryOp op, std::shared_ptr<Expression> operand);
      ~Unary() override = default;
      void print(int level = 0) override;
    };

    class Constant final : public Expression {
      using  const_variant = std::variant<
        int, double
      >;
      const_variant value;
    public:
      explicit Constant(const_variant);
      ~Constant() override = default;
      void print(int level = 0) override;
    };

    namespace arith {

      enum struct ArithmeticType {
        ADD, SUB, DIV, MUL, MOD
      };

      class Arithmetic final : public Binary {
        ArithmeticType type;
        std::shared_ptr<Expression> lhs;
        std::shared_ptr<Expression> rhs;
      public:
        Arithmetic(ArithmeticType, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
        ~Arithmetic() override = default;
        void print(int level = 0) override;
      };

    }


    namespace boolean {

      enum struct BooleanType {
        TRUE, FALSE
      };

      class Boolean final : public Expression {
        BooleanType type;
      public:
        explicit Boolean(BooleanType);
        ~Boolean() override = default;
        void print(int level = 0) override;
      };

    }


    namespace rel {

      enum struct RelationalType {
        EQ, NE, GE, LE, GT, LT, LEQ, GEQ
      };

      class Relational final : public Binary {
      public:
        ~Relational() override = default;
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
      expr::Expression* expression;
    public:
      explicit RegisterAssign(expr::Expression* exp);
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

      std::vector<
        std::variant<
          std::shared_ptr<stmt::Statement>,
          std::shared_ptr<func::Function>
        >
      > children ;

    public:
      explicit Module(std::vector<stmt::Statement *> stmts, std::vector<func::Function *> fns);
      ~Module() override;
      void print(int level = 0) override;
    };
  }


  namespace prog {

    class Program final: public Node {
      std::vector<module::Module*> modules;
      std::vector<
        std::shared_ptr<expr::Expression>
      > expressions;
    public:
      //explicit Program(std::vector<module::Module*> modules);
      Program() = default;
      explicit Program(std::vector<std::shared_ptr<expr::Expression>> exps);
      ~Program() override = default;
      void print(int level = 0) override;

      auto add_expression(std::shared_ptr<expr::Expression> e) {
        expressions.emplace_back(e);
      }
    };

  }


}
