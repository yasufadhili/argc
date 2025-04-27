#include <utility>

#include "ast.hh"


using namespace ast::func;

Function::Function(const std::shared_ptr<ident::Identifier> &name, std::vector<std::shared_ptr<param::Parameter> > parameters, const std::shared_ptr<ReturnTypeInfo> &return_type, const std::shared_ptr<stmt::Block>& body)
  : name_(name), params_(std::move(parameters)), return_type_(return_type), body_(body) {

}


void Function::print(const int level) {
  print_indent(level);
  std::cout << "Function \n";
  name_->print(level + 1);

  body_->print(level + 1);

}

void Function::accept(Visitor &) {

}


void ReturnTypeInfo::print(int level) {
  print_indent(level);
  std::cout << "Return TypeInfo \n";
}

void ReturnTypeInfo::accept(Visitor& v) {

}

SingleReturnType::SingleReturnType(std::shared_ptr<ident::TypeIdentifier> id)
  : id_(std::move(id))
{

}

void SingleReturnType::print(const int level) {
  print_indent(level);
  std::cout << "Single Return TypeInfo \n";
}

void SingleReturnType::accept(Visitor& v) {

}

MultipleReturnType::MultipleReturnType(std::vector<std::shared_ptr<ident::TypeIdentifier> > ids)
  : ids_(std::move(ids)){

}

void MultipleReturnType::print(const int level) {
  print_indent(level);
  std::cout << "Multiple Return TypeInfo \n";
}

void MultipleReturnType::accept(Visitor &) {

}

