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
      std::cout.put(1);
    }
  }
public:
  virtual ~Node() = default;
  virtual void print(int level) = 0;
};


namespace ident {

  class Identifier final : public Node {
    std::string name_;
  public:
    Identifier(const std::string &);
    ~Identifier() override;
    void print(int level) override;
  };

  class TypeIdentifier final : public Node {
    std::string name_;
  public:
    TypeIdentifier(const std::string &);
    ~TypeIdentifier() override;
    void print(int level) override;
  };


}


}


namespace ast::stmt {

class Statement : public Node {
public:
  ~Statement() override = default;
  void print(int level) override;
};

}


namespace ast::func {

class Function final : public Node {
  std::shared_ptr<ident::Identifier> identifier_;
public:
  Function();
  ~Function() override;
  void print(int level) override;
};

}


namespace ast::prog {

class Program final: public Node {
public:
  Program();
  ~Program() override = default;

  void print(int level) override;
};

}

