#include "codegen.hh"

using namespace ast;
using namespace llvm;

CodeGenerator::CodeGenerator(llvm_backend::LLVMContext& context)
  : context_(context),
    //module_(std::make_unique<llvm_backend::Module>("ArgonModule", context)),
    builder_(std::make_unique<llvm_backend::IRBuilder<>>(context)) {}

auto CodeGenerator::take_module() -> std::unique_ptr<llvm_backend::Module> {
  return std::move(module_);
}

void CodeGenerator::visit(mod::Module& module) {
  module_ = std::make_unique<llvm::Module>(module.identifier()->name(), context_);
  builder_ = std::make_unique<llvm::IRBuilder<>>(context_);

  for (const auto& function : module.functions()) {
    function->accept(*this);
  }

  // Generate an implicit main if there are top-level statements
  bool has_top_level_statements = !module.statements().empty();
  if (has_top_level_statements) {
    FunctionType* main_type = FunctionType::get(builder_->getInt32Ty(), false);
    Function* main_func = Function::Create(main_type, Function::ExternalLinkage, "main", module_.get());

    BasicBlock* entry = BasicBlock::Create(context_, "entry", main_func);
    builder_->SetInsertPoint(entry);

    named_values_.clear();

    for (const auto& stmt : module.statements()) {
      stmt->accept(*this);
    }

    builder_->CreateRet(builder_->getInt32(0));
  }
}

void CodeGenerator::visit(func::Function&){}
void CodeGenerator::visit(func::Body&){}
void CodeGenerator::visit(func::Parameter&){}
void CodeGenerator::visit(func::ReturnTypeInfo&){}
void CodeGenerator::visit(func::MultipleReturnType&){}
void CodeGenerator::visit(func::SingleReturnType&){}

void CodeGenerator::visit(ident::TypeIdentifier&){}
void CodeGenerator::visit(ident::Identifier&){}

void CodeGenerator::visit(stmt::Statement&){}
void CodeGenerator::visit(stmt::Empty&){}
void CodeGenerator::visit(stmt::Block&){}
void CodeGenerator::visit(stmt::Print&){}
void CodeGenerator::visit(stmt::Return&){}
void CodeGenerator::visit(stmt::VariableDeclaration&){}
void CodeGenerator::visit(stmt::Assignment&){}

void CodeGenerator::visit(expr::Expression&){}
void CodeGenerator::visit(expr::Binary&){}
void CodeGenerator::visit(expr::Unary&){}
void CodeGenerator::visit(expr::Literal&){}
void CodeGenerator::visit(expr::FunctionCall&){}
void CodeGenerator::visit(expr::Variable&){}
