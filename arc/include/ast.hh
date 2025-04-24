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
    static void print_indent(const int level) {
      for (int i = 0; i < level; ++i) {
        std::cout << " ";
      }
    }

  public:
    virtual ~Node() = default;

    virtual void print(int level) = 0;
  };

}


namespace ast::ident {

  class Identifier final : Node{
  public:
    explicit Identifier(const std::string& name) : name_(name) {}
    ~Identifier() override = default;

    void print(int level) override;
    const std::string& name() const { return name_; }

  private:
    std::string name_;
  };

  class TypeIdentifier final : Node {
  public:
    explicit TypeIdentifier(const std::string& name) : name_(name) {}
    ~TypeIdentifier() override = default;

    void print(int level) override;
    const std::string& name() const { return name_; }

  private:
    std::string name_;
  };

}

namespace ast::expr {

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
      int, double, bool, std::string, char
    >;
    const_variant value;
    sym::TypeKind kind;
  public:
    explicit Constant(const const_variant &, sym::TypeKind);
    ~Constant() override = default;
    void print(int level = 0) override;
  };

  class Variable final : public Expression {
    std::shared_ptr<ident::Identifier> identifier;
    std::shared_ptr<sym::Type> type;
  public:
    //Variable(std::string name, std::shared_ptr<sym::Type> type)
    //    : identifier(std::move(name)), var_type(std::move(type)) {}
    Variable(const std::string&);
    auto get_name() const -> std::string {
      return identifier->name();
    }

    void print(int) override;
  };

}

namespace ast::expr::arith {

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

namespace ast::expr::boolean {

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

namespace ast::expr::rel {

  enum struct RelationalType {
    EQ, NEQ,  GT, LT, GEQ, LEQ
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

namespace ast::stmt {
  class Statement : public Node {
  public:
    ~Statement() override = default;

    void print(int level) override;
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
    //std::shared_ptr<expr::Variable> target;
    std::string target;
    std::shared_ptr<expr::Expression> value;
  public:
    Assignment(std::string target_var, std::shared_ptr<expr::Expression> assigned_value);

    void print(int level = 0) override;
  };

  class ExpressionStatement final : public Statement {
    std::shared_ptr<expr::Expression> expression;
  public:
    explicit ExpressionStatement(std::shared_ptr<expr::Expression>);
    void print(int) override;
  };

  class Return final : public Statement {
    std::optional<std::shared_ptr<expr::Expression>> expression;
  public:
    explicit Return(std::optional<std::shared_ptr<expr::Expression>> expr);
    ~Return() override = default;
    void print(int) override;
  };

}


namespace ast::param {

  class Parameter final : Node {
  public:
    Parameter(std::shared_ptr<ident::Identifier> name,
             std::shared_ptr<ident::TypeIdentifier> type);
    ~Parameter() override = default;

    void print(int level) override;
    const std::shared_ptr<ident::Identifier>& name() const;
    const std::shared_ptr<ident::TypeIdentifier>& type() const;

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::shared_ptr<ident::TypeIdentifier> type_;
  };

}


namespace ast::func {

  class FunctionBody final : Node {
  public:
    explicit FunctionBody(std::vector<std::shared_ptr<stmt::Statement> > statements);

    ~FunctionBody() override = default;

    void print(int level) override;
    auto statements() const -> const std::vector<std::shared_ptr<stmt::Statement> >& ;

  private:
    std::vector<std::shared_ptr<stmt::Statement> > statements_;
  };


  class Function final : Node {
  public:
    // Constructor for function with no return type
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<stmt::Block> body);

    // Constructor for function with single return type
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<ident::TypeIdentifier> return_type,
             std::shared_ptr<stmt::Block> body);

    // Constructor for function with multiple return types
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::vector<std::shared_ptr<ident::TypeIdentifier> > return_types,
             std::shared_ptr<stmt::Block> body);

    // Generic constructor that can handle optional return types
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::optional<std::variant<
               std::shared_ptr<ident::TypeIdentifier>,
               std::vector<std::shared_ptr<ident::TypeIdentifier> >
             > > return_type,
             std::shared_ptr<stmt::Block> body);

    ~Function() override = default;

    const std::shared_ptr<ident::Identifier>& name() const;
    const std::vector<std::shared_ptr<param::Parameter> >& params() const;

    const std::optional<std::variant<
      std::shared_ptr<ident::TypeIdentifier>,
      std::vector<std::shared_ptr<ident::TypeIdentifier> >
    > > &return_type() const;

    auto body() const -> const std::shared_ptr<stmt::Block>&;

    void print(int level) override;

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::vector<std::shared_ptr<param::Parameter> > params_;
    std::optional<std::variant<
      std::shared_ptr<ident::TypeIdentifier>,
      std::vector<std::shared_ptr<ident::TypeIdentifier> >
    > > return_type_;
    std::shared_ptr<stmt::Block> body_;
  };


}


namespace ast::prog {
  class Program final : public Node {
    std::vector<std::shared_ptr<func::Function> > functions_;

  public:
    explicit Program(std::vector<std::shared_ptr<func::Function> >);

    ~Program() override = default;

    void print(int level) override;
  };
}
