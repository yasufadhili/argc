#include "ast.hh"

namespace ast {

  class x86_64_CodeGenerator final : public Visitor {
    
  public:
    void visit(unit::TranslationUnit&) override;
    void visit(mod::Module&) override;
    void visit(func::Function&) override;
    void visit(expr::FunctionCall&) override;
    void visit(func::Parameter&) override;
    void visit(func::SingleReturnType&) override;
    void visit(func::MultipleReturnType&) override;
    void visit(func::Body&) override;
    void visit(func::ReturnTypeInfo&) override;
    void visit(ident::Identifier&) override;
    void visit(ident::TypeIdentifier&) override;
    void visit(stmt::Statement&) override;
    void visit(stmt::Empty&) override;
    void visit(stmt::Block&) override;
    void visit(stmt::Return&) override;
    void visit(stmt::Print&) override;
    void visit(stmt::VariableDeclaration&) override;
    void visit(stmt::Assignment&) override;
    void visit(expr::Expression&) override;
    void visit(expr::Literal&) override;
    void visit(expr::Binary&) override;
    void visit(expr::Unary&) override;
    void visit(expr::Variable&) override;
  };

}
