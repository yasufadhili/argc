#pragma once
#include <map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>

#include "ast.hh"

namespace llvm_backend = llvm;

namespace ast {

class CodeGenerator final : public Visitor {
  llvm_backend::LLVMContext& context_;
  std::unique_ptr<llvm_backend::Module> module_;
  std::unique_ptr<llvm_backend::IRBuilder<>> builder_;
  std::map<std::string, llvm_backend::Value*> named_values_;
  llvm::Type* map_type(const sym_table::Type& type);
public:
  explicit CodeGenerator(llvm_backend::LLVMContext& context);
  ~CodeGenerator() override = default;

  // Accessors
  auto take_module() -> std::unique_ptr<llvm_backend::Module>;

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