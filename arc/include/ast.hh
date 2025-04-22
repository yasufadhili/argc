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
    static void print_indent(int level) {
      for (int i = 0; i < level; ++i) {
        std::cout << "  ";
      }
    }
  public:
    virtual ~Node() = default;
    virtual void print(int level = 0) = 0;
  };

  namespace stmt {

    class Statement : public Node {
    public:
      ~Statement() override = default;
      void print(int level = 0) override;
    };

  }


  namespace func {

  }



  namespace prog {

    class Program : public Node {
    public:
      Program();
      ~Program() override = default;

      void print(int level = 0);
    };

  }


}
