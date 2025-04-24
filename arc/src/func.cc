#include <utility>

#include "../include/ast.hh"


using namespace ast::func;

FunctionBody::FunctionBody(std::vector<std::shared_ptr<stmt::Statement> > statements)
      : statements_(std::move(statements)) {
}

auto FunctionBody::statements() const -> const std::vector<std::shared_ptr<stmt::Statement> > & {
  return statements_;
}

void FunctionBody::print(int level) {
  std::cout << "Function Body \n";
}


Function::Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<stmt::Block> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        body_(std::move(body)) {
}

Function::Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::shared_ptr<ident::TypeIdentifier> return_type,
             std::shared_ptr<stmt::Block> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {
}

Function::Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::vector<std::shared_ptr<ident::TypeIdentifier> > return_types,
             std::shared_ptr<stmt::Block> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_types)),
        body_(std::move(body)) {
}

Function::Function(std::shared_ptr<ident::Identifier> name,
             std::vector<std::shared_ptr<param::Parameter> > params,
             std::optional<std::variant<
               std::shared_ptr<ident::TypeIdentifier>,
               std::vector<std::shared_ptr<ident::TypeIdentifier> >
             > > return_type,
             std::shared_ptr<stmt::Block> body)
      : name_(std::move(name)),
        params_(std::move(params)),
        return_type_(std::move(return_type)),
        body_(std::move(body)) {
}

const std::shared_ptr<ast::ident::Identifier>& Function::name() const { return name_; }
const std::vector<std::shared_ptr<ast::param::Parameter> >& Function::params() const { return params_; }

const std::optional<std::variant<
  std::shared_ptr<ast::ident::TypeIdentifier>,
  std::vector<std::shared_ptr<ast::ident::TypeIdentifier> >
> > &Function::return_type() const { return return_type_; }

auto Function::body() const -> const std::shared_ptr<stmt::Block>& { return body_; }


void Function::print(const int level) {
  print_indent(level);
  std::cout << "Function \n";
  name_->print(level + 1);

  for (const auto& p : params()) {
    print_indent(level);
    p->print(level + 1);
  }

  if (return_type().has_value()) {
    if (std::holds_alternative<std::shared_ptr<ident::TypeIdentifier>>(return_type().value())) {
      auto i = std::get<std::shared_ptr<ident::TypeIdentifier>>(return_type().value());
      if (i == nullptr) {
        return;
      }
      i->print(level + 2);
    } else if (std::holds_alternative<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(return_type().value())) {
      for (const auto& rt: std::get<std::vector<std::shared_ptr<ident::TypeIdentifier>>>(return_type().value())) {
        rt->print(level + 2);
      }
    }
  }

}



