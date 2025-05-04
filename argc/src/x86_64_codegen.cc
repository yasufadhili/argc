#include "x86_64_codegen.hh"

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

