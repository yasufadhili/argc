#include <stack>
#include "ast.hh"

namespace ast {

class x86_64_CodeGenerator final : public Visitor {
  std::stringstream output_;
  std::string current_module_; // Track current module for function prefixing
  std::unordered_map<std::string, int> var_offsets_; // Map variable names to stack offsets
  std::unordered_map<std::string, std::shared_ptr<sym_table::Type>> var_types_; // Variable types
  int current_stack_offset_ = 0; // Track stack allocation
  int label_counter_ = 0; // For generating unique labels
  std::stack<std::string> loop_labels_; // For break/continue statements (if you add them later)
  std::shared_ptr<mod::Module> current_module_node_;

  auto generate_label() -> std::string {
    return ".L" + std::to_string(label_counter_++);
  }

  auto get_type_size(const std::shared_ptr<sym_table::Type>& type) -> std::string {
    if (type->to_string() == "bool") return "1"; 
    if (type->to_string() == "i8") return "1"; 
    if (type->to_string() == "i16") return "2"; 
    if (type->to_string() == "i32") return "4"; 
    if (type->to_string() == "i64") return "8"; 
    if (type->to_string() == "u8") return "1"; 
    if (type->to_string() == "u16") return "2"; 
    if (type->to_string() == "u32") return "4"; 
    if (type->to_string() == "u64") return "8"; 
    if (type->to_string() == "f32") return "4"; 
    if (type->to_string() == "f64") return "8"; 
    return "8"; // Default to 64-bits
}

auto align_stack() -> void {
  output_ << "  # Align stack to 16 bytes\n";
  output_ << "  mov %rsp, %rax\n";
  output_ << "  and $-16, %rsp\n";
}

auto op_to_asm(BinaryOp op) -> std::string {
  switch(op) {
    case BinaryOp::ADD: return "add";
    case BinaryOp::SUB: return "sub";
    case BinaryOp::MUL: return "imul";
    case BinaryOp::DIV: return "idiv"; // Requires special handling
    case BinaryOp::MOD: return "idiv"; // Uses remainder from div
    case BinaryOp::B_AND: return "and";
    case BinaryOp::B_OR: return "or";
    case BinaryOp::L_AND: return "and"; // Logical operations need special handling
    case BinaryOp::L_OR: return "or";   // with conditional jumps
    default: return "nop";
  }
}

auto relop_to_asm(RelationalOp op) -> std::string {
  switch(op) {
    case RelationalOp::LT: return "jl";
    case RelationalOp::GT: return "jg";
    case RelationalOp::EQ: return "je";
    case RelationalOp::LEQ: return "jle";
    case RelationalOp::GEQ: return "jge";
    case RelationalOp::NEQ: return "jne";
    default: return "nop";
  }
}

auto handle_relational_op(RelationalOp op) -> void {
  output_ << "  cmp %rax, %rbx\n";
  std::string true_label = generate_label();
  std::string end_label = generate_label();
  
  output_ << "  " << relop_to_asm(op) << " " << true_label << "\n";
  output_ << "  mov $0, %rax\n";  // False case
  output_ << "  jmp " << end_label << "\n";
  output_ << true_label << ":\n";
  output_ << "  mov $1, %rax\n";  // True case
  output_ << end_label << ":\n";
}
  
public:

  x86_64_CodeGenerator() = default;
  auto get_asm_code() -> std::string const { return output_.str(); }
  auto write_to(std::ostream& os) -> void { os << output_.str(); }
  auto generate_main_function() -> void ;

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
