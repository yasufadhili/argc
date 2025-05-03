#pragma once

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <sstream>
#include <variant>
#include "config.hh"
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
public:
  Node() : location_() {}
  explicit Node(const yy::location& loc) : location_(loc) {}
  virtual ~Node() = default;
  virtual void accept(Visitor&) = 0;
  void set_location(const yy::location& loc) { location_ = loc; }
  const yy::location& location() const { return location_; }

  auto location() -> yy::location {
    return location_;
  }
};

}

namespace ast::ident {

class Identifier final : public Node {
  std::string name_;
public:
  explicit Identifier(std::string name) : name_(std::move(name)) {}
  ~Identifier() override = default;
  void accept(Visitor &) override;
  auto name() const -> std::string { return name_; };
};

class TypeIdentifier final : public Node {
  std::string name_;
public:
  explicit TypeIdentifier(std::string name) : name_(std::move(name)) {}
  ~TypeIdentifier() override = default;
  void accept(Visitor &) override;
  auto name() const -> std::string { return name_; };
};

}

namespace ast::expr {

  using LiteralVariant = std::variant<
    int64_t, double, bool, u_int64_t
  >;

  class Expression : public Node {
  protected:
    std::shared_ptr<sym_table::Type> type_;
  public:
    virtual ~Expression () override = default;
    virtual void accept (Visitor &) override;
    virtual auto type() const -> std::shared_ptr<sym_table::Type> { return type_; }
    virtual void set_type(std::shared_ptr<sym_table::Type> t) { type_ = std::move(t); }
  };

  class Binary final : public Expression {
    std::variant<BinaryOp, RelationalOp> op_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Binary(const std::variant<BinaryOp, RelationalOp> op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
    : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
    ~Binary () override = default;
    void accept (Visitor &) override;
    auto lhs () const -> std::shared_ptr<Expression> { return lhs_; };
    auto rhs () const -> std::shared_ptr<Expression> { return rhs_; };
    auto op () const -> std::variant<BinaryOp, RelationalOp> { return op_; };
  };

  class Unary final : public Expression {
    UnaryOp op_;
    std::shared_ptr<Expression> operand_;
  public:
    Unary(const UnaryOp op, std::shared_ptr<Expression> operand)
    : op_(op), operand_(std::move(operand)) {}
    ~Unary() override = default;
    void accept(Visitor &) override;
    auto operand() const -> std::shared_ptr<Expression> { return operand_; };
    auto op() const -> UnaryOp { return op_; };
  };

  class Literal final : public Expression {
    LiteralVariant value_;
  public:
    explicit Literal(const LiteralVariant v)
    : value_(v){}
    ~Literal() override = default;
    void accept(Visitor &) override;
    auto value() const -> LiteralVariant { return value_; };
  };

  class Variable final : public Expression {
    std::shared_ptr<ident::Identifier> identifier_;
    std::shared_ptr<sym_table::Type> type_;
  public:
    Variable(std::shared_ptr<ident::Identifier> id, std::shared_ptr<sym_table::Type> t)
    : identifier_(std::move(id)), type_(std::move(t)) { }
    ~Variable() override = default;
    void accept(Visitor &) override;
    auto identifier() const -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto type() const -> std::shared_ptr<sym_table::Type> { return type_; }
    auto set_type(std::shared_ptr<sym_table::Type> t) -> void {
      type_ = std::move(t);
    }
  };

  class FunctionCall final : public Expression {
    std::shared_ptr<ident::Identifier> function_;
    std::vector<std::shared_ptr<Expression>> arguments_;
  public:
    FunctionCall(std::shared_ptr<ident::Identifier>, std::vector<std::shared_ptr<Expression>>);
    auto function() const -> std::shared_ptr<ident::Identifier> { return function_; }
    auto arguments() const -> const std::vector<std::shared_ptr<Expression>>& { return arguments_; }
  };
}

namespace ast::stmt {
  class Statement : public Node {
  public:
    void accept (Visitor &) override;
  };

  class Empty final : public Statement {
  public:
    explicit Empty() = default;
    ~Empty() override = default;
    void accept(Visitor &) override;
  };

  class Block final : public Statement {
    std::vector<std::shared_ptr<Statement>> statements_;
  public:
    explicit Block(std::vector<std::shared_ptr<Statement>> stmts) : statements_(std::move(stmts)) {}
    ~Block() override = default;
    void accept(Visitor &) override;
    auto statements() const -> std::vector<std::shared_ptr<Statement>> {
      return statements_;
    }
  };

  class Return final : public Statement {
    std::optional<std::shared_ptr<expr::Expression>> expression_;
  public:
    explicit Return(std::optional<std::shared_ptr<expr::Expression> > expr) : expression_(std::move(expr)){}
    ~Return() override = default;
    void accept(Visitor&) override;
    auto expression() const -> std::optional<std::shared_ptr<expr::Expression>> {return expression_; }
  };

  class VariableDeclaration final : public Statement {
    std::shared_ptr<ident::Identifier> identifier_;
    std::shared_ptr<sym_table::Symbol> symbol_;
    std::shared_ptr<sym_table::Type> type_;
    std::optional<std::shared_ptr<expr::Expression>> initialiser_;
    bool is_const_;
  public:
    VariableDeclaration (
      std::shared_ptr<ident::Identifier> id,
      std::shared_ptr<sym_table::Type> t,
      std::optional<std::shared_ptr<expr::Expression>> init
    ) :
    identifier_(std::move(id)),
    type_(std::move(t)),
    initialiser_(std::move(init)) {}
    ~VariableDeclaration() override = default;
    void accept(Visitor&) override;
    auto identifier() -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto initialiser() -> std::optional<std::shared_ptr<expr::Expression>> { return initialiser_; }
    auto type() -> std::shared_ptr<sym_table::Type> { return type_; }
    auto symbol() -> std::shared_ptr<sym_table::Symbol> { return symbol_; }
    auto set_symbol(std::shared_ptr<sym_table::Symbol> sym) -> void { symbol_ = std::move(sym); }
    auto is_const() const -> bool { return is_const_; }
  };

  class Assignment final : public Statement {
    std::shared_ptr<ident::Identifier> target_;
    std::shared_ptr<expr::Expression> value_;
  public:
    Assignment ( std::shared_ptr<ident::Identifier> target, std::shared_ptr<expr::Expression> val)
     : target_(std::move(target)), value_(std::move(val)) {}
    ~Assignment() override = default;
    void accept(Visitor&) override;
    auto target() -> std::shared_ptr<ident::Identifier> { return target_; }
    auto value() -> std::shared_ptr<expr::Expression> { return value_; }
  };

  class Print final : public Statement {
    std::shared_ptr<expr::Expression> expression_;
  public:
    explicit Print(std::shared_ptr<expr::Expression> e) : expression_(std::move(e)) {}
    ~Print () override = default;
    void accept(Visitor&) override;
    auto expression() -> std::shared_ptr<expr::Expression> { return expression_; }
  };

}

namespace ast::func {

  class Parameter final : public Node {
    std::shared_ptr<ident::Identifier> identifier_;
    std::shared_ptr<ident::TypeIdentifier> type_;
  public:
    Parameter(std::shared_ptr<ident::Identifier> id, std::shared_ptr<ident::TypeIdentifier> t)
    : identifier_(std::move(id)), type_(std::move(t)) {}
    ~Parameter() override = default;
    void accept(Visitor&) override;
    auto identifier () const -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto type () const -> std::shared_ptr<ident::TypeIdentifier> { return type_; }
  };

  class ReturnTypeInfo : public Node {
  public:
    ReturnTypeInfo() = default;
    ~ReturnTypeInfo() override = default;
    void accept(Visitor&) override;
  };

  class SingleReturnType final : public ReturnTypeInfo {
    std::shared_ptr<ident::TypeIdentifier> identifier_;
  public:
    explicit SingleReturnType(std::shared_ptr<ident::TypeIdentifier> id)
    : identifier_(std::move(id)) {}
    ~SingleReturnType() override = default;
    void accept(Visitor&) override;
    auto identifier () const -> std::shared_ptr<ident::TypeIdentifier> { return identifier_;}
  };

  class MultipleReturnType final: public ReturnTypeInfo {
    std::vector<std::shared_ptr<ident::TypeIdentifier>> identifiers_;
  public:
    explicit MultipleReturnType(std::vector<std::shared_ptr<ident::TypeIdentifier>> ids)
    : identifiers_(std::move(ids)) {}
    ~MultipleReturnType() override = default;
    void accept(Visitor&) override;
    auto identifiers () const -> std::vector<std::shared_ptr<ident::TypeIdentifier>> { return identifiers_; }
  };

  class Body final : public Node {
    std::vector<std::shared_ptr<stmt::Statement>> statements_;
  public:
    explicit Body(std::vector<std::shared_ptr<stmt::Statement>> stmts) 
    : statements_ (std::move(stmts)) {}
    ~Body() override = default;
    void accept(Visitor&) override;
    auto statements () const -> std::vector<std::shared_ptr<stmt::Statement>> { return statements_; }
  };

  class Function : public Node {
    std::shared_ptr<ident::Identifier> identifier_;
    std::vector<std::shared_ptr<Parameter>> parameters_;
    std::shared_ptr<ReturnTypeInfo> return_type_;
    std::shared_ptr<Body> body_;
    bool is_public_;
  public:
    Function(
      std::shared_ptr<ident::Identifier> id,
      std::vector<std::shared_ptr<Parameter>> params,
      std::shared_ptr<ReturnTypeInfo> rt,
      std::shared_ptr<Body> b,
      bool is_public = false
    ) : identifier_(std::move(id)), parameters_(std::move(params)), return_type_(std::move(rt)), body_(std::move(b)) {}
    void accept(Visitor&) override;
    auto name() const -> std::shared_ptr<ident::Identifier> { return identifier_; }
    auto parameters() const -> const std::vector<std::shared_ptr<Parameter>>& { return parameters_; }
    auto return_type() const -> std::shared_ptr<ReturnTypeInfo> { return return_type_; }
    auto body() const -> std::shared_ptr<Body> { return body_; }
    auto is_public() const -> bool { return is_public_; }
  };

}

namespace ast::mod {

  class Module final : public Node {
    std::shared_ptr<ident::Identifier> identifier_;
    std::vector<std::shared_ptr<func::Function>> functions_;
    std::vector<std::shared_ptr<stmt::Statement>> statements_;
  public:
    Module (
      std::shared_ptr<ident::Identifier> id,
      std::vector<std::shared_ptr<stmt::Statement>> stmts,
      std::vector<std::shared_ptr<func::Function>> fns
    ) : identifier_(id), functions_(fns), statements_(stmts) {}
  
    Module (
      std::shared_ptr<ident::Identifier> id,
      std::vector<std::shared_ptr<stmt::Statement>> stmts
    ) : identifier_(id), statements_(stmts) {}
  
    Module (
      std::shared_ptr<ident::Identifier> id,
      std::vector<std::shared_ptr<func::Function>> fns
    ) : identifier_(id), functions_(fns) {}
  
    ~Module () override = default;
    void accept (Visitor&) override;
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
    explicit TranslationUnit(std::vector<std::shared_ptr<mod::Module> > m) : modules_(std::move(m)) {}
    ~TranslationUnit() override = default;
    void accept(Visitor &) override;
    auto add_module (std::shared_ptr<mod::Module> m) -> void { modules_.emplace_back(m); }
    auto modules () -> std::vector<std::shared_ptr<mod::Module>> { return modules_; }
  };

}

namespace ast {

  class Visitor {
  protected:  
    config::Config config_;
  public:
    virtual ~Visitor() = default;
    virtual void visit (unit::TranslationUnit&) = 0;

    virtual void visit (mod::Module&) = 0;

    virtual void visit (func::Function&) = 0;
    virtual void visit (func::Parameter&) = 0;
    virtual void visit (func::Body&) = 0;
    virtual void visit (func::ReturnTypeInfo&) = 0;
    virtual void visit (func::SingleReturnType&) = 0;
    virtual void visit (func::MultipleReturnType&) = 0;

    virtual void visit (ident::Identifier&) = 0;
    virtual void visit (ident::TypeIdentifier&) = 0;

    virtual void visit (stmt::Statement&) = 0;
    virtual void visit (stmt::Empty&) = 0;
    virtual void visit (stmt::Block&) = 0;
    virtual void visit (stmt::Return&) = 0;
    virtual void visit (stmt::Print&) = 0;
    virtual void visit (stmt::VariableDeclaration&) = 0;
    virtual void visit (stmt::Assignment&) = 0;

    virtual void visit (expr::Expression&) = 0;
    virtual void visit (expr::Literal&) = 0;
    virtual void visit (expr::Binary&) = 0;
    virtual void visit (expr::Unary&) = 0;
    virtual void visit (expr::Variable&) = 0;

  };

}

namespace ast {
  inline void unit::TranslationUnit::accept(Visitor &v) { v.visit(*this); }

  inline void mod::Module::accept(Visitor &v) { v.visit(*this); }

  inline void func::Function::accept(Visitor &v) { v.visit(*this); }
  inline void func::Body::accept(Visitor &v) { v.visit(*this); }
  inline void func::Parameter::accept(Visitor &v) { v.visit(*this); }
  inline void func::ReturnTypeInfo::accept(Visitor &v) { v.visit(*this); }
  inline void func::MultipleReturnType::accept(Visitor &v) { v.visit(*this); }
  inline void func::SingleReturnType::accept(Visitor &v) { v.visit(*this); }

  inline void ident::TypeIdentifier::accept(Visitor &v) { v.visit(*this); }
  inline void ident::Identifier::accept(Visitor &v) { v.visit(*this); }

  inline void stmt::Statement::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::Block::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::Empty::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::Return::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::Print::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::Assignment::accept(Visitor &v) { v.visit(*this); }
  inline void stmt::VariableDeclaration::accept(Visitor &v) { v.visit(*this); }

  inline void expr::Expression::accept(Visitor &v) { v.visit(*this); }
  inline void expr::Unary::accept(Visitor &v) { v.visit(*this); }
  inline void expr::Binary::accept(Visitor &v) { v.visit(*this); }
  inline void expr::Literal::accept(Visitor &v) { v.visit(*this); }
  inline void expr::Variable::accept(Visitor &v) { v.visit(*this); }
}

namespace ast {

class Printer final : public Visitor {
  int indent_level_ = 0;

  void print_indent() const {
    for (int i = 0; i < indent_level_; ++i) {
      std::cout << " ";
    }
  }

public:
  void visit(unit::TranslationUnit&) override;
  void visit(mod::Module&) override;
  void visit(func::Function&) override;
  void visit(func::Parameter&) override;
  void visit(func::SingleReturnType&) override;
  void visit(func::MultipleReturnType&) override;
  void visit(func::Body&) override;
  void visit(func::ReturnTypeInfo&) override;
  void visit(ident::Identifier&) override;
  void visit(ident::TypeIdentifier&) override;
  void visit(stmt::Statement&) override;
  void visit(stmt::Empty&) override;
  void visit(stmt::Block&) override;
  void visit(stmt::Return&) override;
  void visit(stmt::Print&) override;
  void visit(stmt::VariableDeclaration&) override;
  void visit(stmt::Assignment&) override;
  void visit(expr::Expression&) override;
  void visit(expr::Literal&) override;
  void visit(expr::Binary&) override;
  void visit(expr::Unary&) override;
  void visit(expr::Variable&) override;
};
  
}

namespace ast {

class SymbolCollector final : public Visitor {
  std::shared_ptr<sym_table::SymbolTable> symbol_table_;
  bool error_occurred_;
public:
  SymbolCollector () ;
  ~SymbolCollector () override = default;

  auto has_errors () const -> bool { return error_occurred_; }

  void visit(unit::TranslationUnit&) override;
  void visit(mod::Module&) override;
  void visit(func::Function&) override;
  void visit(func::Parameter&) override;
  void visit(func::SingleReturnType&) override;
  void visit(func::MultipleReturnType&) override;
  void visit(func::Body&) override;
  void visit(func::ReturnTypeInfo&) override;
  void visit(ident::Identifier&) override;
  void visit(ident::TypeIdentifier&) override;
  void visit(stmt::Statement&) override;
  void visit(stmt::Empty&) override;
  void visit(stmt::Block&) override;
  void visit(stmt::Return&) override;
  void visit(stmt::Print&) override;
  void visit(stmt::VariableDeclaration&) override;
  void visit(stmt::Assignment&) override;
  void visit(expr::Expression&) override;
  void visit(expr::Literal&) override;
  void visit(expr::Binary&) override;
  void visit(expr::Unary&) override;
  void visit(expr::Variable&) override;

};

}

namespace ast {

class SemanticAnalyser final : public Visitor {
  std::shared_ptr<sym_table::SymbolTable> symbol_table_;
  bool error_occurred_;
  std::shared_ptr<sym_table::Type> current_return_type_;
public:
  SemanticAnalyser() : error_occurred_(false) {}
  ~SemanticAnalyser() override = default;

  bool has_errors() const { return error_occurred_; }
  
  void visit(unit::TranslationUnit&) override;
  void visit(mod::Module&) override;
  void visit(func::Function&) override;
  void visit(func::Parameter&) override;
  void visit(func::SingleReturnType&) override;
  void visit(func::MultipleReturnType&) override;
  void visit(func::Body&) override;
  void visit(func::ReturnTypeInfo&) override;
  void visit(ident::Identifier&) override;
  void visit(ident::TypeIdentifier&) override;
  void visit(stmt::Statement&) override;
  void visit(stmt::Empty&) override;
  void visit(stmt::Block&) override;
  void visit(stmt::Return&) override;
  void visit(stmt::Print&) override;
  void visit(stmt::VariableDeclaration&) override;
  void visit(stmt::Assignment&) override;
  void visit(expr::Expression&) override;
  void visit(expr::Literal&) override;
  void visit(expr::Binary&) override;
  void visit(expr::Unary&) override;
  void visit(expr::Variable&) override;

};

}