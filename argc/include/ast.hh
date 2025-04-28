#pragma once

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>
#include <sstream> 

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
    Expression() = default;
    ~Expression() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
    virtual int evaluate();
  };

  class Binary : public Expression {
  public:
    ~Binary() override = default;
    void accept(Visitor&) override = 0;
    void print(int level) override;
    int evaluate() override;
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
    int evaluate() override;
  };

  class Constant final : public Expression {
    using  const_variant = std::variant<
      int, double, bool, std::string, char
    >;
    const_variant value_;
    sym::Type::TypeKind kind_;
  public:
    explicit Constant(const_variant , sym::Type::TypeKind);
    ~Constant() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
    int evaluate() override;
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
    auto evaluate() -> int;
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
    int evaluate() override;
  };

}

namespace ast::expr::arith {

  enum struct ArithmeticType {
    ADD, SUB, DIV, MUL, MOD
  };

  class Arithmetic final : public Binary {
    ArithmeticType type_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Arithmetic(ArithmeticType, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
    ~Arithmetic() override = default;
    void print(int level) override;
    void accept(Visitor&) override;
    int evaluate() override;
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
    int evaluate() override;
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
    int evaluate() override;
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
    std::vector<std::shared_ptr<Statement>> statements_;
  public:
    explicit Block(std::vector<std::shared_ptr<Statement>> stmts)
      : statements_(std::move(stmts)) {}

    auto add_statement(std::shared_ptr<Statement> stmt) -> void {
      statements_.push_back(std::move(stmt));
    }
    void print(int level) override;
    void accept(Visitor&) override;
    auto statements() const -> const std::vector<std::shared_ptr<Statement>>& { return statements_; }
  };

  class VariableDeclaration final : public Statement {
    std::string name_;
    std::shared_ptr<sym::Type> type_;
    std::optional<std::shared_ptr<expr::Expression>> initialiser_;
    std::shared_ptr<sym::Symbol> symbol_;
  public:
    VariableDeclaration(
      std::string name,
      const std::shared_ptr<sym::Type> &type,
      const std::optional<std::shared_ptr<expr::Expression>>& init
    );

    void print(int level) override;
    void accept(Visitor&) override;
    auto set_symbol(std::shared_ptr<sym::Symbol> sym) -> void { symbol_ = std::move(sym); }
    auto get_symbol() const -> std::shared_ptr<sym::Symbol> { return symbol_; }
    auto name() const -> const std::string& { return name_; }
    auto type() const -> std::shared_ptr<sym::Type> { return type_; }
    auto initialiser() -> const std::optional<std::shared_ptr<expr::Expression>>& { return initialiser_; }
  };

  class Assignment final : public Statement {
    //std::shared_ptr<expr::Variable> target;
    std::string target_;
    std::shared_ptr<expr::Expression> value_;
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
    std::optional<std::shared_ptr<expr::Expression>> expression_;
  public:
    explicit Return(std::optional<std::shared_ptr<expr::Expression>> expr);
    ~Return() override = default;
    void accept(Visitor&) override;
    void print(int) override;
    auto expression() -> std::optional<std::shared_ptr<expr::Expression>>& { return expression_; }
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
    auto name() const -> std::shared_ptr<ident::Identifier> { return name_; }
    auto body() -> std::shared_ptr<stmt::Block>& { return body_; }

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
    const std::string& name() const { return name_; }
    auto functions() -> std::vector<std::shared_ptr<func::Function>>& { return functions_; }
    void add_function(const std::shared_ptr<func::Function>& func);
  };

  class Program final : public Node {
    std::vector<std::shared_ptr<Module> > modules_;

  public:
    explicit Program(std::vector<std::shared_ptr<Module>>);
    Program() = default;
    ~Program() override = default;
    void accept(Visitor&) override;
    void print(int level) override;
    auto modules() -> std::vector<std::shared_ptr<Module>> { return modules_; }
  };
}

namespace ast {

class Visitor {
  std::stringstream output_;

public:

  auto emit(std::string code) -> void;

  auto generate_label(int value) -> std::string;

  auto get_output() -> std::stringstream&;

  virtual ~Visitor() = default;
  virtual void visit (std::shared_ptr<prog::Program>& p) { p->accept(*this);}
  virtual void visit (std::shared_ptr<prog::Module>& m) { m->accept(*this);}
  virtual void visit (std::shared_ptr<func::Function>& f) { f->accept(*this); }
  virtual void visit (std::shared_ptr<func::ReturnTypeInfo>& rti) { rti->accept(*this); }
  virtual void visit (std::shared_ptr<func::SingleReturnType>& rti) { rti->accept(*this); }
  virtual void visit (std::shared_ptr<func::MultipleReturnType>& rti) { rti->accept(*this); }
  virtual void visit (std::shared_ptr<param::Parameter>& p) { p->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::Statement>& s) { s->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::Block>& b) { b->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::ExpressionStatement>& s) { s->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::Assignment>& a) { a->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::Repeat>& r) { r->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::Return>& r) { r->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::VariableDeclaration>& vd) { vd->accept(*this); }
  virtual void visit (std::shared_ptr<stmt::EmptyStatement>& es) { es->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Expression>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::arith::Arithmetic>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::rel::Relational>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Variable>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Unary>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Logical>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Binary>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Constant>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::Bitwise>& e) { e->accept(*this); }
  virtual void visit (std::shared_ptr<expr::boolean::Boolean>& e) { e->accept(*this); }

};

}
