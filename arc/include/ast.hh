#pragma once

#include <memory>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>

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


  class Identifier : public Node {
    std::string name;
  public:
    auto set_name(const std::string& n) -> void {
      name = n;
    }
    auto get_name() const -> std::string {
      return name;
    }
    void print(const int level = 0) override {
      print_indent(level);
      std::cout << "Identifier: " << get_name() << std::endl;
    }
  };

  namespace expr {

    class Expression : public Node {
    public:
      ~Expression() override = default;
      void print(int level = 0) override;
    };

    class Variable final : public Expression {
      std::shared_ptr<Identifier> identifier;
      std::shared_ptr<sym::Type> type;
    public:
      //Variable(std::string name, std::shared_ptr<sym::Type> type)
      //    : identifier(std::move(name)), var_type(std::move(type)) {}
      Variable(const std::string&);
      auto get_name() const -> std::string { 
        return identifier->get_name(); 
      }

      void print(int) override;
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
        int, double, bool, std::string
      >;
      const_variant value;
      sym::Type::TypeKind kind;
    public:
      explicit Constant(const const_variant, sym::Type::TypeKind);
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
        TRUE = 1, FALSE = 0
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
        EQ, NEQ, GEQ, LEQ, GT, LT
      };

      class Relational final : public Binary {
        RelationalType type;
        std::shared_ptr<Expression> lhs;
        std::shared_ptr<Expression> rhs;
      public:
        Relational(RelationalType, std::shared_ptr<Expression>, std::shared_ptr<Expression> );
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

    class Block final : public Statement {
      std::vector<std::shared_ptr<Statement>> statements;
    public:
      explicit Block(std::vector<std::shared_ptr<Statement>> stmts)
        : statements(std::move(stmts)) {}

      const auto& get_statements() const { return statements; }
      auto add_statement(std::shared_ptr<Statement> stmt) -> void {
        statements.push_back(std::move(stmt));
      }
      void print(int level = 0) override;
    };

    class Initialise final : public Statement {
      std::shared_ptr<expr::Expression> expression;
    public:
      explicit Initialise(std::shared_ptr<expr::Expression>);
      ~Initialise() override = default;
      void print(int level = 0) override;
    };

    class VariableDeclaration final : public Statement {
      std::string name;
      std::shared_ptr<sym::Type> type;
      std::optional<std::shared_ptr<expr::Expression>> initialiser;
      std::shared_ptr<sym::Symbol> symbol;
    public:
      VariableDeclaration(
        std::string name,
        const std::shared_ptr<sym::Type> &type,
        const std::optional<std::shared_ptr<expr::Expression>>& init
      );

      void print(int level) override;

      auto get_name() const -> std::string;
      auto get_type() const -> std::shared_ptr<sym::Type>;
      //auto get_initialiser() const -> std::shared_ptr<expr::Expression>;

      //auto set_symbol(std::shared_ptr<sym::Symbol> sym) -> void;
      //auto get_symbol() const -> std::shared_ptr<sym::Symbol>;

    };

    class Assignment final : public Statement {
      std::shared_ptr<expr::Variable> target;
      std::shared_ptr<expr::Expression> value;
    public:
      Assignment(std::shared_ptr<expr::Variable> target_var, std::shared_ptr<expr::Expression> assigned_value)
          : target(std::move(target_var)),
            value(std::move(assigned_value)) {}

      void print(int level = 0) override;
    };

    class RegisterAssign final : public Statement {
      std::shared_ptr<expr::Expression> expression;
    public:
      explicit RegisterAssign(expr::Expression* exp);
      ~RegisterAssign() override;
      void print(int level = 0) override;
    };

    class ExpressionStatement final : public Statement {
      std::shared_ptr<expr::Expression> expression;
    public:
      explicit ExpressionStatement(std::shared_ptr<expr::Expression>);
      void print(int) override;
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
      std::vector<std::shared_ptr<stmt::Statement>> statements;
    public:
      Program() = default;
      explicit Program(std::vector<std::shared_ptr<stmt::Statement>> stmts);
      ~Program() override = default;
      void print(int level = 0) override;

      auto add_statement(std::shared_ptr<stmt::Statement> s) {
        statements.emplace_back(s);
      }
    };

  }


}
