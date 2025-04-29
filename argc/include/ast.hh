#pragma once

#include "analyser.hh"
#include "codegen.hh"

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
  virtual void accept(analyser::SemanticAnalyser&) = 0;
  virtual void accept(codegen::x86_64_CodeGenerator&) = 0;
  virtual void print(int level) = 0;
};

}

namespace ast::ident {
class Identifier final : public Node {
  std::string name_;
public:
  explicit Identifier(std::string);
  ~Identifier() override = default;
  void accept(analyser::SemanticAnalyser &) override;
  void accept(codegen::x86_64_CodeGenerator &) override;
  void print(int level) override;
  auto name() const -> std::string { return name_; };
};

class TypeIdentifier final : public Node {
  std::string name_;
public:
  explicit TypeIdentifier(std::string);
  ~TypeIdentifier() override = default;
  void accept(analyser::SemanticAnalyser &) override;
  void accept(codegen::x86_64_CodeGenerator &) override;
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
    ~TranslationUnit() override = default;
    void accept(analyser::SemanticAnalyser &) override;
    void accept(codegen::x86_64_CodeGenerator &) override;
    void print(int level) override;
  };
}
