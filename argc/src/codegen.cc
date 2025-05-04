#include "ast.hh"

using namespace ast;
using namespace llvm;

CodeGenerator::CodeGenerator(llvm_backend::LLVMContext& context)
  : context_(context),
    //module_(std::make_unique<llvm_backend::Module>("ArgonModule", context)),
    builder_(std::make_unique<llvm_backend::IRBuilder<>>(context)) {}

auto CodeGenerator::take_module() -> std::unique_ptr<llvm_backend::Module> {
  return std::move(module_);
}

void CodeGenerator::visit(unit::TranslationUnit& tu) {
  for (auto& mod : tu.modules()) {
    mod->accept(*this);
  }
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