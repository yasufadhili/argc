#include "../include/ast.hh"


using namespace ast::param;

Parameter::Parameter(std::shared_ptr<ident::Identifier> name,
             std::shared_ptr<ident::TypeIdentifier> type)
        : name_(std::move(name)), type_(std::move(type)) {}

void Parameter::print(const int level) {
  print_indent(level);
  std::cout << "Parameter \n";
  name_->print(level + 2);
}

void Parameter::accept(Visitor &) {

}
