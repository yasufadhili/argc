#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

namespace ast {

class SemanticAnalyser;
class CodeGenerator;

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
}

namespace ast::stmt {
  class Statement : public Node {};
  class Asm final: public Statement {
    std::string asm_;
  public:
    explicit Asm(std::string);
    ~Asm() override = default;
    void accept(SemanticAnalyser &) override;
    void accept(CodeGenerator &) override;
    void print(int level) override;
    auto assembly() const -> std::string { return asm_; };
  };
}

namespace ast::func {
  class Function : public Node {};
}

namespace ast::mod {
  class Module : public Node {};
}

namespace ast::unit {
  class TranslationUnit final : public Node {
  public:
    TranslationUnit() = default;
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