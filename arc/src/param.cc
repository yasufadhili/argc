#include "../include/ast.hh"


using namespace ast::param;

Parameter::Parameter(std::shared_ptr<ident::Identifier> name,
             std::shared_ptr<ident::TypeIdentifier> type)
        : name_(std::move(name)), type_(std::move(type)) {}

const std::shared_ptr<ast::ident::Identifier>& Parameter::name() const { return name_; }
const std::shared_ptr<ast::ident::TypeIdentifier>& Parameter::type() const { return type_; }

void Parameter::print(const int level) {
  print_indent(level);
  std::cout << "Parameter \n";
}

