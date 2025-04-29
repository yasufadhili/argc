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

namespace ast::expr {
  class Expression : public Node {};
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
  class TranslationUnit : public Node {};
}
