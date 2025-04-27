
#include "../include/ast.hh"

using namespace ast;

void prog::Program::accept(Visitor &) {

}

void func::Function::accept(Visitor &) {

}

void func::ReturnTypeInfo::accept(Visitor &) {

}

void func::SingleReturnType::accept(Visitor &) {}


void func::MultipleReturnType::accept(Visitor &) {

}

void param::Parameter::accept(Visitor &) {

}


void stmt::Statement::accept(Visitor &) {

}

void stmt::EmptyStatement::accept(Visitor &) {

}

void stmt::Block::accept(Visitor &) {

}

void stmt::ExpressionStatement::accept(Visitor &) {

}

void stmt::Assignment::accept(Visitor &) {

}

void stmt::Return::accept(Visitor &) {

}

void stmt::Repeat::accept(Visitor &) {

}

void stmt::VariableDeclaration::accept(Visitor &) {

}

void expr::Expression::accept(Visitor &) {

}

void expr::Binary::accept(Visitor &) {

}

void expr::Unary::accept(Visitor &) {

}

void expr::boolean::Boolean::accept(Visitor &) {

}


void expr::Bitwise::accept(Visitor &) {

}

void expr::Constant::accept(Visitor &) {

}

void expr::arith::Arithmetic::accept(Visitor &) {

}

void expr::rel::Relational::accept(Visitor &) {

}

void expr::Logical::accept(Visitor &) {

}

void expr::Variable::accept(Visitor &) {

}

void ident::Identifier::accept(Visitor &) {

}

void ident::TypeIdentifier::accept(Visitor &) {

}



