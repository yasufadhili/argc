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

void x86_64_CodeGenerator::visit(stmt::Assignment& assign){
  output_ << "    # Assignment to: " << assign.target()->name() << "\n";
        
  assign.value()->accept(*this);  // Result in %rax
  
  // Store in variable location
  auto var_name = assign.target()->name();
  if (var_offsets_.find(var_name) == var_offsets_.end()) {
    std::cerr << "Gen: Error: Variable " << var_name << " not declared" << std::endl; // TODO
    return;
  }
  
  int offset = var_offsets_[var_name];
  output_ << "    mov %rax, " << offset << "(%rbp)\n";
}

void x86_64_CodeGenerator::visit(stmt::Return& ret){
  output_ << "  # Return statement\n";
  if (ret.expression()) {
    (*ret.expression())->accept(*this);
    // Result is already in %rax, which is the return register
  } else {
    // Void return
    output_ << "  xor %rax, %rax\n";
  }
  
  // Jump to function epilogue
  output_ << "  mov %rbp, %rsp\n";
  output_ << "  pop %rbp\n";
  output_ << "  ret\n";
}

void x86_64_CodeGenerator::visit(stmt::Print& print){
  output_ << "  # Print statement\n";
  print.expression()->accept(*this); // Result in %rax
  
  // Determine format string based on expression type
  std::string fmt_label;
  auto expr_type = print.expression()->type();
  
  if (expr_type->to_string() == "i32" || expr_type->to_string() == "i64") {
      fmt_label = "print_int_fmt";
  } else if (expr_type->to_string() == "bool") {
    // Handle boolean specially - convert 0/1 to "false"/"true"
    std::string false_label = generate_label();
    std::string end_label = generate_label();
    output_ << "  cmp $0, %rax\n";
    output_ << "  je " << false_label << "\n";
    output_ << "  lea true_str(%rip), %rdi\n";
    output_ << "  jmp " << end_label << "\n";
    output_ << false_label << ":\n";
    output_ << "  lea false_str(%rip), %rdi\n";
    output_ << end_label << ":\n";
    output_ << "  lea print_bool_fmt(%rip), %rsi\n";
    output_ << "  xor %rax, %rax\n";
    output_ << "  call printf\n";
    return;  // Already handled
  } else if (expr_type->to_string() == "f32" || expr_type->to_string() == "f64") {
    // For floating point, we need to use XMM registers
    fmt_label = "print_double_fmt";
    output_ << "  movq %rax, %xmm0\n";
    output_ << "  lea " << fmt_label << "(%rip), %rdi\n";
    output_ << "  mov $1, %rax\n";  // One floating point arg
    output_ << "  call printf\n";
    return;  // Already handled
  } else {
    // Default to int
    fmt_label = "print_int_fmt";
  }
  
  // Standard printf call
  output_ << "  mov %rax, %rsi\n";
  output_ << "  lea " << fmt_label << "(%rip), %rdi\n";
  output_ << "  xor %rax, %rax\n";  // No floating point args
  output_ << "  call printf\n";
}

void x86_64_CodeGenerator::visit(stmt::VariableDeclaration& var_decl){
  output_ << "  # Variable declaration: " << var_decl.identifier()->name() << "\n";
        
  std::string var_name = var_decl.identifier()->name();
  
  // Allocate space on the stack
  current_stack_offset_ -= 8;  //  All variables use 8 bytes for now
  var_offsets_[var_name] = current_stack_offset_;
  var_types_[var_name] = var_decl.type();
  
  // Initialise if initialiser is present
  if (var_decl.initialiser()) {
    (*var_decl.initialiser())->accept(*this);  // Result in %rax
    output_ << "  mov %rax, " << current_stack_offset_ << "(%rbp)\n";
  }
}

void x86_64_CodeGenerator::visit(expr::Expression& ){}

void x86_64_CodeGenerator::visit(expr::Binary& bin){
  output_ << "  # Binary operation\n";
        
  // Visit left and right expressions
  if (std::holds_alternative<BinaryOp>(bin.op())) {
    BinaryOp op = std::get<BinaryOp>(bin.op());
    
    // Special handling for logical operators
    if (op == BinaryOp::L_AND || op == BinaryOp::L_OR) {
      handle_logical_op(bin, op);
      return;
    }
    
    // Special handling for division and modulus
    if (op == BinaryOp::DIV || op == BinaryOp::MOD) {
      handle_div_mod(bin, op);
      return;
    }
    
    // Standard binary operation
    bin.rhs()->accept(*this);    // Result in %rax
    output_ << "  push %rax\n";  // Save right operand
    bin.lhs()->accept(*this);    // Result in %rax
    output_ << "  pop %rbx\n";   // Get right operand
    
    // Perform operation
    output_ << "  " << op_to_asm(op) << " %rbx, %rax\n";
      
  } else if (std::holds_alternative<RelationalOp>(bin.op())) {
    RelationalOp op = std::get<RelationalOp>(bin.op());
    
    bin.rhs()->accept(*this);    // Result in %rax
    output_ << "  push %rax\n";  // Save right operand
    bin.lhs()->accept(*this);    // Result in %rax
    output_ << "  mov %rax, %rbx\n"; // Move left operand to %rbx
    output_ << "  pop %rax\n";   // Get right operand
    
    handle_relational_op(op);
  }
}

auto x86_64_CodeGenerator::handle_div_mod(expr::Binary& bin, BinaryOp op) -> void {
  bin.rhs()->accept(*this);    // Result in %rax (divisor)
  output_ << "  mov %rax, %rbx\n";  // Save divisor in %rbx
  bin.lhs()->accept(*this);    // Result in %rax (dividend)
  
  // Sign extend %rax into %rdx:%rax
  output_ << "  cqo\n";  // Convert quadword to octaword
  
  // Perform division: %rdx:%rax / %rbx
  output_ << "  idiv %rbx\n";
  
  // For division, result is in %rax
  // For modulus, remainder is in %rdx
  if (op == BinaryOp::MOD) {
      output_ << "  mov %rdx, %rax\n";  // Move remainder to %rax
  }
}

void x86_64_CodeGenerator::handle_logical_op(expr::Binary& bin, BinaryOp op) {
  // Short-circuit evaluation for logical operators
  std::string short_circuit_label = generate_label();
  std::string end_label = generate_label();
  
  if (op == BinaryOp::L_AND) {
    // Evaluate left operand
    bin.lhs()->accept(*this);
    output_ << "  cmp $0, %rax\n";
    output_ << "  je " << short_circuit_label << "\n";  // If left is false, short-circuit
    
    // Evaluate right operand only if left is true
    bin.rhs()->accept(*this);
    output_ << "  cmp $0, %rax\n";
    output_ << "  movne $1, %rax\n";  // Normalise to 0/1
    output_ << "  jmp " << end_label << "\n";
    
    // Short-circuit case (result is false)
    output_ << short_circuit_label << ":\n";
    output_ << "  xor %rax, %rax\n";  // Set result to false
      
  } else if (op == BinaryOp::L_OR) {
    // Evaluate left operand
    bin.lhs()->accept(*this);
    output_ << "  cmp $0, %rax\n";
    output_ << "  jne " << short_circuit_label << "\n";  // If left is true, short-circuit
    
    // Evaluate right operand only if left is false
    bin.rhs()->accept(*this);
    output_ << "  cmp $0, %rax\n";
    output_ << "  movne $1, %rax\n";  // Normalise to 0/1
    output_ << "  jmp " << end_label << "\n";
    
    // Short-circuit case (result is true)
    output_ << short_circuit_label << ":\n";
    output_ << "  mov $1, %rax\n";  // Set result to true
  }
  
  output_ << end_label << ":\n";
}

void x86_64_CodeGenerator::visit(expr::Unary& un){
  output_ << "  # Unary operation\n";
        
  un.operand()->accept(*this);  // Result in %rax
  
  // Apply operation
  switch (un.op()) {
    case UnaryOp::NEG:
      output_ << "  neg %rax\n";
      break;
    case UnaryOp::B_NOT:
      output_ << "    not %rax\n";
      break;
    case UnaryOp::L_NOT:
      // Logical NOT: !0 = 1, !non-zero = 0
      output_ << "  cmp $0, %rax\n";
      output_ << "  sete %al\n";  // Set if equal to zero
      output_ << "  movzx %al, %rax\n";  // Zero-extend byte to quad word
      break;
  }
}

void x86_64_CodeGenerator::visit(expr::Literal& lit){
  output_ << "  # Literal value\n";
        
  std::visit([this](auto&& value) {
    using T = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<T, int64_t>) {
      output_ << "  mov $" << value << ", %rax\n";
    } else if constexpr (std::is_same_v<T, u_int64_t>) {
        output_ << "  mov $" << value << ", %rax\n";
    } else if constexpr (std::is_same_v<T, bool>) {
        output_ << "  mov $" << (value ? 1 : 0) << ", %rax\n";
    } else if constexpr (std::is_same_v<T, double>) {
      // TODO
      // For floating point, we'd need to handle this differently
      // This is simplified; we'd typically load from a data section
      int64_t bits;
      memcpy(&bits, &value, sizeof(bits));
      output_ << "  # Float value: " << value << "\n";
      output_ << "  mov $" << bits << ", %rax\n";
    }
  }, lit.value());
}

void x86_64_CodeGenerator::visit(expr::Variable& ){}


