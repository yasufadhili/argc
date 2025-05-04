#include "ast.hh"

using namespace ast;

CodeGenerator::CodeGenerator(llvm_backend::LLVMContext& context)
  : context_(context),
    //module_(std::make_unique<llvm_backend::Module>("ArgonModule", context)),
    builder_(std::make_unique<llvm_backend::IRBuilder<>>(context)) {}

auto CodeGenerator::take_module() -> std::unique_ptr<llvm_backend::Module> {
  return std::move(module_);
}

void CodeGenerator::visit(mod::Module& module) {
  // Use module name to name the LLVM module
  std::string module_name = module.identifier()->name();
  module_ = std::make_unique<llvm::Module>(module_name, context_);

  // Create IRBuilder again here (or you can keep one at class level)
  builder_ = std::make_unique<llvm::IRBuilder<>>(context_);

  // We could generate an implicit main if statements are provided at top level
  if (!module.statements().empty()) {
    // Create a main function
    llvm::FunctionType* main_type = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(context_), false);
    llvm::Function* main_func = llvm::Function::Create(
        main_type, llvm::Function::ExternalLinkage, "main", module_.get());

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context_, "entry", main_func);
    builder_->SetInsertPoint(entry);

    named_values_.clear(); // Clear locals

    for (auto& stmt : module.statements()) {
      stmt->accept(*this);
    }

    // End with return 0
    builder_->CreateRet(llvm::ConstantInt::get(context_, llvm::APInt(32, 0)));
  }

  // Now generate code for each function in the module
  for (const auto& function : module.functions()) {
    function->accept(*this);
  }

  // Optional: If your language allows top-level statements,
  // you could generate an implicit main() function here.
}

void CodeGenerator::visit(func::Function& function) {
  // Map parameter types
  std::vector<llvm::Type*> arg_types;
  for (const auto& param : function.parameters()) {
    arg_types.push_back(map_type(*param->type()));
  }

  // Create function type: assuming return type is int32 for now
  llvm::FunctionType* func_type =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(context_), arg_types, false);

  // Create the function in the module
  llvm::Function* llvm_func =
      llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                             function.name()->name(), module_.get());

  // Create entry block and set builder insert point
  llvm::BasicBlock* entry = llvm::BasicBlock::Create(context_, "entry", llvm_func);
  builder_->SetInsertPoint(entry);

  // Reset local symbol table (named_values_)
  named_values_.clear();

  // Assign arguments to symbol table
  unsigned idx = 0;
  for (auto& arg : llvm_func->args()) {
    arg.setName(function.parameters()[idx++]->identifier()->name());
    named_values_[arg.getName().str()] = &arg;
  }

  // Visit function body
  function.body()->accept(*this);

  // Ensure function ends with a return
  if (builder_->GetInsertBlock()->empty() ||
      !builder_->GetInsertBlock()->back().isTerminator()) {
    builder_->CreateRet(llvm::ConstantInt::get(context_, llvm::APInt(32, 0)));
  }
}