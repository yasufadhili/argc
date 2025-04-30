#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

namespace ast {

class SemanticAnalyser;
class CodeGenerator;

enum class BinaryOp { ADD, SUB, MUL, DIV, MOD };
enum class UnaryOp { NEG, B_NOT, L_NOT };
enum class RelationalOp {LT, GT, EQ, LEQ, GEQ, NEQ };

class Node {
protected:
  static void print_indent(const int level) {
    for (int i = 0; i < level; ++i) {
      std::cout << " ";
    }
  }
public:
  virtual ~Node() = default;
  virtual void accept(SemanticAnalyser&) = 0;
  virtual void accept(CodeGenerator&) = 0;
  virtual void print(int level) = 0;
};

}

namespace ast::ident {
class Identifier final : public Node {
  std::string name_;
public:
  explicit Identifier(std::string);
  ~Identifier() override = default;
  void accept(SemanticAnalyser &) override;
  void accept(CodeGenerator &) override;
  void print(int level) override;
  auto name() const -> std::string { return name_; };
};

class TypeIdentifier final : public Node {
  std::string name_;
public:
  explicit TypeIdentifier(std::string);
  ~TypeIdentifier() override = default;
  void accept(SemanticAnalyser &) override;
  void accept(CodeGenerator &) override;
  void print(int level) override;
  auto name() const -> std::string { return name_; };
};
}

namespace ast::expr {

  class Expression : public Node {

  };

  class Binary final : public Expression {
    BinaryOp op_;
    std::shared_ptr<Expression> lhs_;
    std::shared_ptr<Expression> rhs_;
  public:
    Binary(BinaryOp, std::shared_ptr<Expression>, std::shared_ptr<Expression>);
    ~Binary() override = default;
    void accept(SemanticAnalyser &) override;
    void accept(CodeGenerator &) override;
    void print(int level) override;
    auto lhs() const -> std::shared_ptr<Expression> { return lhs_; };
    auto rhs() const -> std::shared_ptr<Expression> { return rhs_; };
    auto op() const -> BinaryOp { return op_; };
  };

  class Unary final : public Expression {
    UnaryOp op_;
    std::shared_ptr<Expression> operand_;
  public:
    Unary(UnaryOp, std::shared_ptr<Expression>);
    ~Unary() override = default;
    void accept(SemanticAnalyser &) override;
    void accept(CodeGenerator &) override;
    void print(int level) override;
    auto operand() const -> std::shared_ptr<Expression> { return operand_; };
    auto op() const -> UnaryOp { return op_; };
  };

  class Literal final : public Expression {
    int value_;
  public:
    explicit Literal(int);
    ~Literal() override = default;
    void accept(SemanticAnalyser &) override;
    void accept(CodeGenerator &) override;
    void print(int level) override;
    auto value() const -> int { return value_; };
  };
}

namespace ast::stmt {
  class Statement : public Node {};
}

namespace ast::func {
  class Function : public Node {};
}

namespace ast::mod {
  class Module : public Node {};
}

namespace ast::unit {
  class TranslationUnit final : public Node {
    std::shared_ptr<expr::Expression> expr_;
  public:
    TranslationUnit(std::shared_ptr<expr::Expression>);
    ~TranslationUnit() override = default;
    void accept(SemanticAnalyser &) override;
    void accept(CodeGenerator &) override;
    void print(int level) override;
  };
}

namespace ast {

struct SematicError {
  std::string message;
  explicit SematicError(std::string msg)
    : message(std::move(msg)) {}
};

class SemanticAnalyser final {
  std::vector<SematicError> errors_;
public:
  ~SemanticAnalyser() = default;
  SemanticAnalyser() = default;
  auto analyse(std::shared_ptr<unit::TranslationUnit>&) -> bool;
  auto get_errors() -> std::vector<SematicError> { return errors_; }
  auto add_error(const std::string& msg) -> void ;
public:
  void visit(std::shared_ptr<unit::TranslationUnit>&);
};
}

namespace ast {

class CodeGenerator final{
  std::stringstream output_;
public:
  ~CodeGenerator() = default;
  auto get_output() -> std::stringstream & { return output_; }
  auto emit(const std::string&code)-> void {
    output_ << code << "\n";
  }
public:
  void generate(std::shared_ptr<unit::TranslationUnit>&u) {
    u->accept(*this);
  }
};

}