#include "ast.hh"

using namespace ast;

CodeGenerator::CodeGenerator(llvm_backend::LLVMContext& context)
  : context_(context),
    module_(std::make_unique<llvm_backend::Module>("ArgonModule", context)),
    builder_(std::make_unique<llvm_backend::IRBuilder<>>(context)) {}

auto CodeGenerator::take_module() -> std::unique_ptr<llvm_backend::Module> {
  return std::move(module_);
}