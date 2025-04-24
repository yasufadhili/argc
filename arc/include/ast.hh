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
        std::cout << " ";
      }
    }

  public:
    virtual ~Node() = default;

    virtual void print(int level) = 0;
  };

}


namespace ast::ident {

  class Identifier : Node{
  public:
    explicit Identifier(const std::string& name) : name_(name) {}
    ~Identifier() override = default;

    void print(int level) override;
    const std::string& name() const { return name_; }

  private:
    std::string name_;
  };

  class TypeIdentifier : Node {
  public:
    explicit TypeIdentifier(const std::string& name) : name_(name) {}
    ~TypeIdentifier() override = default;

    void print(int level) override;
    const std::string& name() const { return name_; }

  private:
    std::string name_;
  };


}

namespace ast::stmt {
  class Statement : public Node {
  public:
    ~Statement() override = default;

    void print(int level) override;
  };
}


namespace ast::param {

  class Parameter : Node {
  public:
    Parameter(std::shared_ptr<ident::Identifier> name,
             std::shared_ptr<ident::TypeIdentifier> type)
        : name_(std::move(name)), type_(std::move(type)) {}
    ~Parameter() override = default;

    void print(int level) override;
    const std::shared_ptr<ident::Identifier>& name() const { return name_; }
    const std::shared_ptr<ident::TypeIdentifier>& type() const { return type_; }

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::shared_ptr<ident::TypeIdentifier> type_;
  };

}


namespace ast::func {

  class FunctionBody final : Node {
  public:
    explicit FunctionBody(std::vector<std::shared_ptr<stmt::Statement> > statements)
      : statements_(std::move(statements)) {
    }

    ~FunctionBody() override = default;

    void print(int level) override;
    auto statements() const -> const std::vector<std::shared_ptr<stmt::Statement> > & {
      return statements_;
    }

  private:
    std::vector<std::shared_ptr<stmt::Statement> > statements_;
  };


  class Function final : Node {
  public:
    // Constructor for function with no return type
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<FunctionBody> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        body_(std::move(body)) {
    }

    // Constructor for function with single return type
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<ident::TypeIdentifier> return_type,
             std::shared_ptr<FunctionBody> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {
    }

    // Constructor for function with multiple return types
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::vector<std::shared_ptr<ident::TypeIdentifier> > return_types,
             std::shared_ptr<FunctionBody> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_types)),
        body_(std::move(body)) {
    }

    // Generic constructor that can handle optional return types
    Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::optional<std::variant<
               std::shared_ptr<ident::TypeIdentifier>,
               std::vector<std::shared_ptr<ident::TypeIdentifier> >
             > > return_type,
             std::shared_ptr<FunctionBody> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {
    }

    ~Function() override = default;

    const std::shared_ptr<ident::Identifier> &name() const { return name_; }
    const std::vector<std::shared_ptr<param::Parameter> > &params() const { return params_; }

    const std::optional<std::variant<
      std::shared_ptr<ident::TypeIdentifier>,
      std::vector<std::shared_ptr<ident::TypeIdentifier> >
    > > &return_type() const { return return_type_; }

    auto body() const -> const std::shared_ptr<FunctionBody>& { return body_; }
    void print(int level) override;

  private:
    std::shared_ptr<ident::Identifier> name_;
    std::vector<std::shared_ptr<param::Parameter> > params_;
    std::optional<std::variant<
      std::shared_ptr<ident::TypeIdentifier>,
      std::vector<std::shared_ptr<ident::TypeIdentifier> >
    > > return_type_;
    std::shared_ptr<FunctionBody> body_;
  };


}


namespace ast::prog {
  class Program final : public Node {
    std::vector<std::shared_ptr<func::Function> > functions_;

  public:
    explicit Program(std::vector<std::shared_ptr<func::Function> >);

    ~Program() override = default;

    void print(int level) override;
  };
}
