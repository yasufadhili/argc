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
