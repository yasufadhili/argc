#include "x86_64_codegen.hh"
#include "ast.hh"

using namespace ast;

void x86_64_CodeGenerator::visit(unit::TranslationUnit& unit) {
  output_ << "  .text\n";
  output_ << "  .global main\n\n";
  
  output_ << "  .extern printf\n";
  output_ << "  .extern exit\n\n";
  
  // Data section for string constants
  output_ << "  .section .rodata\n";
  output_ << "print_int_fmt: .string \"%ld\\n\"\n";
  output_ << "print_bool_fmt: .string \"%s\\n\"\n";
  output_ << "print_double_fmt: .string \"%f\\n\"\n";
  output_ << "true_str: .string \"true\"\n";
  output_ << "false_str: .string \"false\"\n\n";
  output_ << "  .text\n\n";
  
  for (auto& module : unit.modules()) {
    module->accept(*this);
  }
}

void x86_64_CodeGenerator::visit(mod::Module& module) {
  current_module_ = module.identifier()->name();
  current_module_node_ = std::make_shared<mod::Module>(module);
  
  // Process global statements first (if any)
  if (!module.statements().empty()) {
    // Generate a special "init" function for module-level statements
    output_ << current_module_ << "_init:\n";
    output_ << "  push %rbp\n";
    output_ << "  mov %rsp, %rbp\n";
    
    // Reset stack tracking for this function
    var_offsets_.clear();
    current_stack_offset_ = 0;
    
    for (auto& stmt : module.statements()) {
        stmt->accept(*this);
    }
    
    // Function epilogue
    output_ << "  mov %rbp, %rsp\n";
    output_ << "  pop %rbp\n";
    output_ << "  ret\n\n";
  }
  
  for (auto& func : module.functions()) {
    func->accept(*this);
  }
  
  // If this is the main module, generate the main function
  if (current_module_ == "main" || current_module_ == "Main") {
    generate_main_function();
  }
}

auto x86_64_CodeGenerator::generate_main_function() -> void {
  output_ << "main:\n";
  output_ << "  push %rbp\n";
  output_ << "  mov %rsp, %rbp\n";
  
  // Call initialisation function if there were global statements
  if (!current_module_node_->statements().empty()) {
    output_ << "  call " << current_module_ << "_init\n";
  }
  
  // Call the Main module's main function if it exists
  bool has_main_func = false;
  for (auto& func : current_module_node_->functions()) {
    if (func->name()->name() == "main") {
      has_main_func = true;
      output_ << "  call " << current_module_ << "_main\n";
      break;
    }
  }
  
  // Return 0 from main
  output_ << "  mov $0, %rax\n";
  output_ << "  mov %rbp, %rsp\n";
  output_ << "  pop %rbp\n";
  output_ << "  ret\n\n";
}

void x86_64_CodeGenerator::visit(func::Function& func){
  // Reset stack tracking for this function
  var_offsets_.clear();
  current_stack_offset_ = 0;

  output_ << current_module_ << "_" << func.name()->name() << ":\n";
  output_ << "  push %rbp\n";
  output_ << "  mov %rsp, %rbp\n";
  
  // Allocate space for parameters
  int param_offset = 16; // First param is at 16(%rbp),  64-bit for now
  for (auto& param : func.parameters()) {
    param->accept(*this);
    std::string param_name = param->identifier()->name();
    var_offsets_[param_name] = param_offset;
    var_types_[param_name] = param->type();
    param_offset += 8; // All parameters are 8 bytes for now
  }

  if (func.body()) {
    func.body()->accept(*this);
  }
  
  // Function epilogue
  output_ << "  mov %rbp, %rsp\n";
  output_ << "  pop %rbp\n";
  output_ << "  ret\n\n";
}

void x86_64_CodeGenerator::visit(expr::FunctionCall& call){
  // Save registers that might be overwritten by the call
  output_ << "  # Function call: " << call.function()->name() << "\n";
  output_ << "  push %rdi\n";
  output_ << "  push %rsi\n";
  output_ << "  push %rdx\n";
  output_ << "  push %rcx\n";
  output_ << "  push %r8\n";
  output_ << "  push %r9\n";
  
  // Evaluate arguments in reverse order and push them onto the stack
  const auto& args = call.arguments();
  for (auto it = args.rbegin(); it != args.rend(); ++it) {
    (*it)->accept(*this); // Result will be in %rax
    output_ << "  push %rax\n";
  }
  
  // Pop arguments into the correct registers
  std::vector<std::string> reg_names = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
  int reg_idx = 0;
  
  for (size_t i = 0; i < std::min(args.size(), reg_names.size()); ++i) {
    output_ << "  pop " << reg_names[reg_idx++] << "\n";
  }
  
  // TODO : Handle any additional args still on stack if more than 6 arguments
  
  // Ensure 16-byte stack alignment before the call
  align_stack();
  
  // Call the function
  std::string full_func_name;
  // Check if we're calling a function from this module or another
  if (call.function()->name().find('.') != std::string::npos) {
    // Calling function from another module
    full_func_name = call.function()->name();
  } else {
    // Calling function from current module
    full_func_name = current_module_ + "_" + call.function()->name();
  }
  
  output_ << "  call " << full_func_name << "\n";
  
  // Restore registers
  output_ << "  pop %r9\n";
  output_ << "  pop %r8\n";
  output_ << "  pop %rcx\n";
  output_ << "  pop %rdx\n";
  output_ << "  pop %rsi\n";
  output_ << "  pop %rdi\n";
  
  // Result is in %rax
}

void x86_64_CodeGenerator::visit(func::Body& body){
  for (auto& stmt : body.statements()) {
    stmt->accept(*this);
  }
}

void x86_64_CodeGenerator::visit(func::Parameter& ){ /** Handled in function visit */ }
void x86_64_CodeGenerator::visit(func::ReturnTypeInfo&){}
void x86_64_CodeGenerator::visit(func::SingleReturnType& ){}
void x86_64_CodeGenerator::visit(func::MultipleReturnType& ){}

void x86_64_CodeGenerator::visit(ident::TypeIdentifier& ){}
void x86_64_CodeGenerator::visit(ident::Identifier& ){}

void x86_64_CodeGenerator::visit(stmt::Statement& ){}

void x86_64_CodeGenerator::visit(stmt::Empty& ){
  output_ << "  # Empty statement\n";
}

void x86_64_CodeGenerator::visit(stmt::Block& block){
  output_ << "  # Block start\n";
  for (auto& stmt : block.statements()) {
    stmt->accept(*this);
  }
  output_ << "  # Block end\n";
}

void x86_64_CodeGenerator::visit(stmt::Assignment& ){}
void x86_64_CodeGenerator::visit(stmt::Return& ){}
void x86_64_CodeGenerator::visit(stmt::Print& ){}
void x86_64_CodeGenerator::visit(stmt::VariableDeclaration& ){}

void x86_64_CodeGenerator::visit(expr::Expression& ){}
void x86_64_CodeGenerator::visit(expr::Binary& ){}
void x86_64_CodeGenerator::visit(expr::Unary& ){}
void x86_64_CodeGenerator::visit(expr::Literal& ){}
void x86_64_CodeGenerator::visit(expr::Variable& ){}


