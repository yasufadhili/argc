#pragma once

#include <memory>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>

#include "symbols.hh"


namespace ast {

  class Visitor;

  class Node {
  protected:
    static void print_indent(const int level) {
      for (int i = 0; i < level; ++i) {
        std::cout << " ";
      }
    }

  public:
    virtual ~Node() = default;
    virtual void accept(Visitor&) = 0;
    virtual void print(int level) = 0;
  };

}


namespace ast::ident {

  class Identifier final : Node{
  public:
    explicit Identifier(std::string  name) : name_(std::move(name)) {}
    ~Identifier() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
    const std::string& name() const { return name_; }

  private:
    std::string name_;
  };

  class TypeIdentifier final : Node {
  public:
    explicit TypeIdentifier(const std::string& name) : name_(name) {}
    ~TypeIdentifier() override = default;
    void accept(Visitor&) override;
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
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class Binary : public Expression {
  public:
    ~Binary() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class Unary final : public Expression {
  public:
    enum struct UnaryOp {
      NEG, NOT, LOGICAL_NOT
    };
  private:
    UnaryOp op;
    std::shared_ptr<Expression> operand;

  public:
    Unary(UnaryOp op, std::shared_ptr<Expression> operand);
    ~Unary() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
  };

  class Constant final : public Expression {
    using  const_variant = std::variant<
      int, double, bool, std::string, char
    >;
    const_variant value;
    sym::Type::TypeKind kind;
  public:
    explicit Constant(const_variant , sym::Type::TypeKind);
    ~Constant() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class Variable final : public Expression {
    std::shared_ptr<ident::Identifier> identifier;
    std::shared_ptr<sym::Type> type;
  public:
    //Variable(std::string name, std::shared_ptr<sym::Type> type)
    //    : identifier(std::move(name)), var_type(std::move(type)) {}
    explicit Variable(const std::string&);
    void accept(Visitor&) override;
    void print(int) override;
  };

  enum struct BitwiseOp {
    AND, OR, XOR, NOT, SHL, SHR
  };

  class Bitwise final : public Binary {
    BitwiseOp op_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Bitwise(BitwiseOp op, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
    ~Bitwise() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
  };

  enum struct LogicalOp {
    AND, OR
  };

  class Logical final : public Binary {
    LogicalOp op_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Logical(LogicalOp op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);
    ~Logical() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
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
    void print(int level) override;
    void accept(Visitor&) override;
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
    void print(int level) override;
    void accept(Visitor&) override;
  };

}

namespace ast::expr::rel {

  enum struct RelationalType {
    EQ, NEQ,  GT, LT, GEQ, LEQ, NONE
  };

  class Relational final : public Binary {
    RelationalType type_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Relational(RelationalType, std::shared_ptr<Expression>, std::shared_ptr<Expression> );
    ~Relational() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
  };

}

namespace ast::stmt {
  class Statement : public Node {
  public:
    ~Statement() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class EmptyStatement final : public Statement {
  public:
    EmptyStatement() = default;
    ~EmptyStatement() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
  };

  class Block final : public Statement {
    std::vector<std::shared_ptr<Statement>> statements;
  public:
    explicit Block(std::vector<std::shared_ptr<Statement>> stmts)
      : statements(std::move(stmts)) {}

    auto add_statement(std::shared_ptr<Statement> stmt) -> void {
      statements.push_back(std::move(stmt));
    }
    void print(int level) override;
    void accept(Visitor&) override;
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
    void accept(Visitor&) override;
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
    ~Assignment() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class ExpressionStatement final : public Statement {
    std::shared_ptr<expr::Expression> expression;
  public:
    explicit ExpressionStatement(std::shared_ptr<expr::Expression>);
    ~ExpressionStatement() override = default;
    void accept(Visitor&) override;
    void print(int) override;
  };

  class Return final : public Statement {
    std::optional<std::shared_ptr<expr::Expression>> expression;
  public:
    explicit Return(std::optional<std::shared_ptr<expr::Expression>> expr);
    ~Return() override = default;
    void accept(Visitor&) override;
    void print(int) override;
  };

  class Repeat final : public Statement {
    std::optional<std::shared_ptr<expr::Expression>> times_;
    std::shared_ptr<Block> body_;
  public:
    explicit Repeat(std::optional<std::shared_ptr<expr::Expression>> times);
    ~Repeat() override = default;
    void accept(Visitor&) override;
    void print(int) override;
  };

}


namespace ast::param {

  class Parameter final : Node {
  public:
    Parameter(std::shared_ptr<ident::Identifier> name,
             std::shared_ptr<ident::TypeIdentifier> type);
    ~Parameter() override = default;
    void accept(Visitor&) override;
    void print(int level) override;

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::shared_ptr<ident::TypeIdentifier> type_;
  };

}


namespace ast::func {

  class ReturnTypeInfo : public Node {
  public:
    ReturnTypeInfo() = default;
    ~ReturnTypeInfo() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class SingleReturnType final : public ReturnTypeInfo {
    std::shared_ptr<ident::TypeIdentifier> id_;
  public:
    explicit SingleReturnType(std::shared_ptr<ident::TypeIdentifier> id);
    ~SingleReturnType() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class MultipleReturnType final: public ReturnTypeInfo {
    std::vector<std::shared_ptr<ident::TypeIdentifier>> ids_;
  public:
    explicit MultipleReturnType(std::vector<std::shared_ptr<ident::TypeIdentifier>> ids);
    ~MultipleReturnType() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };


  class Function final : Node {
  public:
    Function(
      const std::shared_ptr<ident::Identifier> &name,
      std::vector<std::shared_ptr<param::Parameter>> parameters,
      const std::shared_ptr<ReturnTypeInfo> &return_type,
      const std::shared_ptr<stmt::Block>& body
    );
    ~Function() override = default;
    void print(int level) override;
    void accept(Visitor&) override;

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::vector<std::shared_ptr<param::Parameter>> params_;
    std::shared_ptr<ReturnTypeInfo> return_type_;
    std::shared_ptr<stmt::Block> body_;
  };


}


namespace ast::prog {

  class Module final : public Node {
    std::string name_;
    std::vector<std::shared_ptr<func::Function> > functions_;
  public:
    Module(std::string name, std::vector<std::shared_ptr<func::Function>> functions);
    ~Module() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };

  class Program final : public Node {
    std::vector<std::shared_ptr<Module> > modules_;

  public:
    explicit Program(std::vector<std::shared_ptr<Module>>);
    Program() = default;
    ~Program() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
  };
}

namespace ast {

class Visitor {
public:

  virtual ~Visitor() = default;
  virtual void visit (prog::Program& p) { p.accept(*this);}
  virtual void visit (func::Function& f) { f.accept(*this); }
  virtual void visit (func::ReturnTypeInfo& rti) { rti.accept(*this); }
  virtual void visit (func::SingleReturnType& rti) { rti.accept(*this); }
  virtual void visit (func::MultipleReturnType& rti) { rti.accept(*this); }
  virtual void visit (param::Parameter& p) { p.accept(*this); }
  virtual void visit (stmt::Statement& s) { s.accept(*this); }
  virtual void visit (stmt::Block& b) { b.accept(*this); }
  virtual void visit (stmt::ExpressionStatement& s) { s.accept(*this); }
  virtual void visit (stmt::Assignment& a) { a.accept(*this); }
  virtual void visit (stmt::Repeat& r) { r.accept(*this); }
  virtual void visit (stmt::Return& r) { r.accept(*this); }
  virtual void visit (stmt::VariableDeclaration& vd) { vd.accept(*this); }
  virtual void visit (stmt::EmptyStatement& es) { es.accept(*this); }
  virtual void visit (expr::Expression& e) { e.accept(*this); }
  virtual void visit (expr::arith::Arithmetic& e) { e.accept(*this); }
  virtual void visit (expr::rel::Relational& e) { e.accept(*this); }
  virtual void visit (expr::Variable& e) { e.accept(*this); }
  virtual void visit (expr::Unary& e) { e.accept(*this); }
  virtual void visit (expr::Logical& e) { e.accept(*this); }
  virtual void visit (expr::Binary& e) { e.accept(*this); }
  virtual void visit (expr::Constant& e) { e.accept(*this); }
  virtual void visit (expr::Bitwise& e) { e.accept(*this); }
  virtual void visit (expr::boolean::Boolean& e) { e.accept(*this); }

};

}
