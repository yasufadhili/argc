#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <sstream>
#include <variant>
#include "error_handler.hh"
#include "location.hh"
#include "sym_table.hh"
#include "util_logger.hh"

namespace ast {

class Visitor;

enum class BinaryOp { ADD, SUB, MUL, DIV, MOD, B_AND, B_OR, L_AND, L_OR, NONE };
enum class UnaryOp { NEG, B_NOT, L_NOT };
enum class RelationalOp {LT, GT, EQ, LEQ, GEQ, NEQ };

class Node {
  yy::location location_;
protected:
  auto location() -> yy::location {
    return location_;
  }
  static void print_indent(const int level) {
    for (int i = 0; i < level; ++i) {
      std::cout << " ";
    }
  }
public:
  Node() : location_() {}
  explicit Node(const yy::location& loc) : location_(loc) {}
  virtual ~Node() = default;
  virtual void accept(Visitor&) = 0;
  virtual void print(int level) = 0;
  void set_location(const yy::location& loc) { location_ = loc; }
  const yy::location& location() const { return location_; }
};

}

namespace ast::ident {

class Identifier final : public Node {
  std::string name_;
public:
  explicit Identifier(std::string);
  ~Identifier() override = default;
  void accept(Visitor &) override;
  void print(int level) override;
  auto name() const -> std::string { return name_; };
};

class TypeIdentifier final : public Node {
  std::string name_;
public:
  explicit TypeIdentifier(std::string);
  ~TypeIdentifier() override = default;
  void accept(Visitor &) override;
  
  void print(int level) override;
  auto name() const -> std::string { return name_; };
};

}

namespace ast::expr {

  using LiteralVariant = std::variant<
    int64_t, double, bool, u_int64_t
  >;

  class Expression : public Node {};

  class Binary final : public Expression {
    std::variant<BinaryOp, RelationalOp> op_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Binary (std::variant<BinaryOp, RelationalOp>, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
    ~Binary () override = default;
    void accept (Visitor &) override;
    void print(int level) override;
    auto lhs () const -> std::shared_ptr<Expression> { return lhs_; };
    auto rhs () const -> std::shared_ptr<Expression> { return rhs_; };
    auto op () const -> std::variant<BinaryOp, RelationalOp> { return op_; };
  };

  class Unary final : public Expression {
    UnaryOp op_;
    std::shared_ptr<Expression> operand_;
  public:
    Unary(UnaryOp, std::shared_ptr<Expression>);
    ~Unary() override = default;
    void accept(Visitor &) override;
    
    void print(int level) override;
    auto operand() const -> std::shared_ptr<Expression> { return operand_; };
    auto op() const -> UnaryOp { return op_; };
  };

  class Literal final : public Expression {
    LiteralVariant value_;
  public:
    explicit Literal(LiteralVariant);
    ~Literal() override = default;
    void accept(Visitor &) override;
    void print(int level) override;
    auto value() const -> LiteralVariant { return value_; };
  };

  class Variable final : public Expression {
    std::shared_ptr<ident::Identifier> identifier_;
    std::shared_ptr<sym_table::Type> type_;
  public:
    Variable(std::shared_ptr<ident::Identifier>, std::shared_ptr<sym_table::Type>);
    ~Variable() override = default;
    void accept(Visitor &) override;
    void print(int level) override;
    auto identifier() const -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto type() const -> std::shared_ptr<sym_table::Type> { return type_; }
    auto set_type(std::shared_ptr<sym_table::Type> t) -> void {
      type_ = std::move(t);
    }
  };
}

namespace ast::stmt {
  class Statement : public Node {};

  class Empty final : public Statement {
  public:
    explicit Empty() = default;
    ~Empty() override = default;
    void accept(Visitor &) override {};
    void print(int level) override;
  };

  class Block final : public Statement {
    std::vector<std::shared_ptr<Statement>> statements_;
  public:
    explicit Block(std::vector<std::shared_ptr<Statement>>);
    ~Block() override = default;
    void accept(Visitor &) override;
    void print(int level) override;
    auto statements() const -> std::vector<std::shared_ptr<Statement>> {
      return statements_;
    }
  };

  class Return final : public Statement {
    std::optional<std::shared_ptr<expr::Expression>> expression_;
  public:
    explicit Return(std::optional<std::shared_ptr<expr::Expression>> expr);
    ~Return() override = default;
    void accept(Visitor&) override;
    void print(int) override;
    auto expression() const -> std::optional<std::shared_ptr<expr::Expression>> {return expression_; }
  };

  class VariableDeclaration final : public Statement {
    std::shared_ptr<ident::Identifier> identifier_;
    std::shared_ptr<sym_table::Symbol> symbol_;
    std::shared_ptr<sym_table::Type> type_;
    std::optional<std::shared_ptr<expr::Expression>> initialiser_;
  public:
    VariableDeclaration(
      std::shared_ptr<ident::Identifier>,
      std::shared_ptr<sym_table::Type>,
      std::optional<std::shared_ptr<expr::Expression>>
    );
    ~VariableDeclaration() override = default;
    void accept(Visitor&) override;
    void print(int) override;
    auto identifier() -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto initialiser() -> std::optional<std::shared_ptr<expr::Expression>> { return initialiser_; }
    auto type() -> std::shared_ptr<sym_table::Type> { return type_; }
    auto symbol() -> std::shared_ptr<sym_table::Symbol> { return symbol_; }
    auto set_symbol(std::shared_ptr<sym_table::Symbol> sym) -> void { symbol_ = std::move(sym); }
  };

  class Assignment final : public Statement {
    std::shared_ptr<ident::Identifier> target_;
    std::shared_ptr<expr::Expression> value_;
  public:
    Assignment(
      std::shared_ptr<ident::Identifier>,
      std::shared_ptr<expr::Expression>
    );
    ~Assignment() override = default;
    void accept(Visitor&) override;
    void print(int) override;
    auto target() -> std::shared_ptr<ident::Identifier> { return target_; }
    auto value() -> std::shared_ptr<expr::Expression> { return value_; }
  };

  class Print final : public Statement {
    std::shared_ptr<expr::Expression> expression_;
  public:
    explicit Print(std::shared_ptr<expr::Expression>);
    ~Print () override = default;
    void accept(Visitor&) override;
    void print(int) override;
    auto expression() -> std::shared_ptr<expr::Expression> { return expression_; }
  };

}

namespace ast::func {

  class Function : public Node {};

}

namespace ast::mod {

  class Module final : public Node {
    std::shared_ptr<ident::Identifier> identifier_;
    std::vector<std::shared_ptr<func::Function>> functions_;
    std::vector<std::shared_ptr<stmt::Statement>> statements_;
  public:
    Module (
      std::shared_ptr<ident::Identifier>,
      std::vector<std::shared_ptr<func::Function>>,
      std::vector<std::shared_ptr<stmt::Statement>>
    ) ;
    Module (
      std::shared_ptr<ident::Identifier>,
      std::vector<std::shared_ptr<stmt::Statement>>
    ) ;
    Module (
      std::shared_ptr<ident::Identifier>,
      std::vector<std::shared_ptr<func::Function>>
    ) ;
    ~Module () override = default;
    void accept (Visitor&) override;
    void print (int) override;
    auto identifier () -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto functions () -> std::vector<std::shared_ptr<func::Function>> { return functions_; }
    auto statements () -> std::vector<std::shared_ptr<stmt::Statement>> { return statements_; }
    auto add_function (std::shared_ptr<func::Function> f) -> void { functions_.emplace_back(f); }
    auto add_statement (std::shared_ptr<stmt::Statement> s) -> void { statements_.emplace_back(s); }
  };

}

namespace ast::unit {

  class TranslationUnit final : public Node {
    std::vector<std::shared_ptr<mod::Module>> modules_;
  public:
    explicit TranslationUnit(std::vector<std::shared_ptr<mod::Module>>);
    ~TranslationUnit() override = default;
    void accept(Visitor &) override;
    void print(int level) override;
    auto add_module (std::shared_ptr<mod::Module> m) -> void { modules_.emplace_back(m); }
    auto modules () -> std::vector<std::shared_ptr<mod::Module>> { return modules_; }
  };

}

namespace ast {

  class Visitor {
  public:
    virtual ~Visitor() = default;
    virtual void visit (std::shared_ptr<unit::TranslationUnit>&) = 0;

    virtual void visit (std::shared_ptr<mod::Module>&) = 0;

    virtual void visit (std::shared_ptr<func::Function>&) = 0;

    virtual void visit (std::shared_ptr<ident::Identifier>&) = 0;
    virtual void visit (std::shared_ptr<ident::TypeIdentifier>&) = 0;

    virtual void visit (std::shared_ptr<stmt::Statement>&) = 0;
    virtual void visit (std::shared_ptr<stmt::Empty>&) = 0;
    virtual void visit (std::shared_ptr<stmt::Block>&) = 0;
    virtual void visit (std::shared_ptr<stmt::Return>&) = 0;
    virtual void visit (std::shared_ptr<stmt::Print>&) = 0;
    virtual void visit (std::shared_ptr<stmt::VariableDeclaration>&) = 0;
    virtual void visit (std::shared_ptr<stmt::Assignment>&) = 0;

    virtual void visit (std::shared_ptr<expr::Expression>&) = 0;
    virtual void visit (std::shared_ptr<expr::Literal>&) = 0;
    virtual void visit (std::shared_ptr<expr::Binary>&) = 0;
    virtual void visit (std::shared_ptr<expr::Unary>&) = 0;
    virtual void visit (std::shared_ptr<expr::Variable>&) = 0;

  };

}