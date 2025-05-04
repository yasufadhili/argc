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