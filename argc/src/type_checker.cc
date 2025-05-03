#include <algorithm>
#include <iostream>
#include <sstream>
#include "ast.hh"

using namespace ast;

TypeChecker::TypeChecker() 
  : error_occurred_(false), 
    symbol_table_(sym_table::SymbolTable::get_instance()) {
  initialise_type_rules();
}

void TypeChecker::initialise_type_rules() {
  // Initialise binary operation type rules
  // Integer operations
  binary_type_rules_.push_back({BinaryOp::ADD, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  binary_type_rules_.push_back({BinaryOp::SUB, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  binary_type_rules_.push_back({BinaryOp::MUL, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  binary_type_rules_.push_back({BinaryOp::DIV, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  binary_type_rules_.push_back({BinaryOp::MOD, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  
  // Floating point operations would promote to highest precision
  // Bitwise operations
  binary_type_rules_.push_back({BinaryOp::B_AND, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  binary_type_rules_.push_back({BinaryOp::B_OR, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "i64"});
  
  // Logical operations
  binary_type_rules_.push_back({BinaryOp::L_AND, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "bool"});
  binary_type_rules_.push_back({BinaryOp::L_OR, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, sym_table::Type::TypeKind::PRIMITIVE, "bool"});
}

bool TypeChecker::is_compatible(const std::shared_ptr<sym_table::Type>& left, 
                              const std::shared_ptr<sym_table::Type>& right) {
  // Null check
  if (!left || !right) {
    return false;
  }
  
  return left->is_compatible_with(*right);
}

std::shared_ptr<sym_table::Type> TypeChecker::get_result_type(
  const std::variant<BinaryOp, RelationalOp>& op,
  const std::shared_ptr<sym_table::Type>& left,
  const std::shared_ptr<sym_table::Type>& right) {
  
  // Check for null types
  if (!left || !right) {
      return nullptr;
  }
  
  // Handle relational operations
  if (std::holds_alternative<RelationalOp>(op)) {
      // All relational operations result in a boolean type
      return sym_table::Type::create_bool_type();
  }
  
  // Get the binary operation
  BinaryOp binary_op = std::get<BinaryOp>(op);
  
  // Special case for string concatenation with +
  if (binary_op == BinaryOp::ADD && 
      left->get_name() == "str" && right->get_name() == "str") {
      return sym_table::Type::create_string_type();
  }
  
  // Handle pointer arithmetic
  if (binary_op == BinaryOp::ADD || binary_op == BinaryOp::SUB) {
      if (left->is_pointer_type() && right->is_integral_type()) {
          return sym_table::Type::create_pointer_type(left->get_base_type());
      }
      
      if (right->is_pointer_type() && left->is_integral_type() && binary_op == BinaryOp::ADD) {
          return sym_table::Type::create_pointer_type(right->get_base_type());
      }
  }
  
  // Handle pointer subtraction
  if (binary_op == BinaryOp::SUB && 
      left->is_pointer_type() && right->is_pointer_type()) {
      // Pointers must have compatible base types
      if (left->get_base_type()->is_compatible_with(*right->get_base_type())) {
          return sym_table::Type::create_integer_type();
      }
      return nullptr;
  }
  
  // Logical operations
  if (binary_op == BinaryOp::L_AND || binary_op == BinaryOp::L_OR) {
      return sym_table::Type::create_bool_type();
  }
  
  // Numeric operations
  if (left->is_numeric_type() && right->is_numeric_type()) {
      // Apply type promotion rules
      return sym_table::Type::get_common_type(*left, *right);
  }
  
  // Bitwise operations
  if ((binary_op == BinaryOp::B_AND || binary_op == BinaryOp::B_OR) &&
      left->is_integral_type() && right->is_integral_type()) {
      return sym_table::Type::get_common_type(*left, *right);
  }
  
  // No compatible operation found
  return nullptr;
}

std::shared_ptr<sym_table::Type> TypeChecker::get_unary_result_type(
    UnaryOp op,
    const std::shared_ptr<sym_table::Type>& operand) {
    
    if (!operand) {
        return nullptr;
    }
    
    switch (op) {
        case UnaryOp::NEG:
            // Negation requires numeric type
            if (operand->is_numeric_type()) {
                return operand;
            }
            break;
        
        case UnaryOp::B_NOT:
            // Bitwise NOT requires integral type
            if (operand->is_integral_type()) {
                return operand;
            }
            break;
            
        case UnaryOp::L_NOT:
            // Logical NOT works on boolean and produces boolean
            if (operand->get_name() == "bool" || operand->is_integral_type()) {
                return sym_table::Type::create_bool_type();
            }
            break;
    }
    
    return nullptr;
}

bool TypeChecker::is_safe_assignment(
    const std::shared_ptr<sym_table::Type>& target,
    const std::shared_ptr<sym_table::Type>& source) {
    
    if (!target || !source) {
        return false;
    }
    
    // Exact match
    if (target->get_name() == source->get_name() && 
        target->get_kind() == source->get_kind()) {
        return true;
    }
    
    // Check if source can be implicitly cast to target
    return source->can_implicitly_cast_to(*target);
}

void TypeChecker::report_type_error(const std::string& message, const yy::location& location,
                             const std::optional<std::string>& code_snippet,
                             const std::optional<std::string>& suggestion) {
    error_occurred_ = true;
    error::DiagnosticHandler::instance().error(message, location, code_snippet, suggestion);
}

std::string TypeChecker::get_type_name(const std::shared_ptr<sym_table::Type>& type) {
    if (!type) {
        return "unknown";
    }
    return type->to_string();
}

std::shared_ptr<sym_table::Type> TypeChecker::get_literal_type(const expr::LiteralVariant& value) {
    if (std::holds_alternative<int64_t>(value)) {
        return sym_table::Type::create_integer_type();
    } else if (std::holds_alternative<double>(value)) {
        return sym_table::Type::create_floating_point_type();
    } else if (std::holds_alternative<bool>(value)) {
        return sym_table::Type::create_bool_type();
    } else if (std::holds_alternative<u_int64_t>(value)) {
        // Unsigned integer, create appropriate type
        auto type = std::make_shared<sym_table::Type>(sym_table::Type::TypeKind::PRIMITIVE, "u64");
        return type;
    }
    
    // Default case (should not happen with well-formed AST)
    return nullptr;
}

bool TypeChecker::is_valid_utf8(const std::string& str) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str.c_str());
    const unsigned char* end = bytes + str.length();
    
    while (bytes < end) {
        // Single byte (ASCII)
        if (bytes[0] <= 0x7F) {
            bytes += 1;
            continue;
        }
        
        // 2-byte sequence
        if ((bytes[0] & 0xE0) == 0xC0) {
            if (bytes + 1 >= end || (bytes[1] & 0xC0) != 0x80) {
                return false;
            }
            bytes += 2;
            continue;
        }
        
        // 3-byte sequence
        if ((bytes[0] & 0xF0) == 0xE0) {
            if (bytes + 2 >= end || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) {
                return false;
            }
            bytes += 3;
            continue;
        }
        
        // 4-byte sequence
        if ((bytes[0] & 0xF8) == 0xF0) {
            if (bytes + 3 >= end || (bytes[1] & 0xC0) != 0x80 || 
                (bytes[2] & 0xC0) != 0x80 || (bytes[3] & 0xC0) != 0x80) {
                return false;
            }
            bytes += 4;
            continue;
        }
        
        // Invalid UTF-8 encoding
        return false;
    }
    
    return true;
}

//============================================================================
// Visitor implementations
//============================================================================

void TypeChecker::visit(unit::TranslationUnit& unit) {
    // Traverse all modules
    for (const auto& module : unit.modules()) {
        module->accept(*this);
    }
}

void TypeChecker::visit(mod::Module& module) {
    // Type check global statements
    for (const auto& stmt : module.statements()) {
        stmt->accept(*this);
    }
    
    // Type check functions
    for (const auto& func : module.functions()) {
        func->accept(*this);
    }
}

void TypeChecker::visit(func::Function& function) {
  // Save current return type
  auto prev_return_type = current_return_type_;
  
  // Set current function's return type
  if (auto single_return = dynamic_cast<func::SingleReturnType*>(function.return_type().get())) {
    std::string type_name = single_return->identifier()->name();
    current_return_type_ = symbol_table_->lookup_type(type_name);
    
    if (!current_return_type_) {
      report_type_error("Return type '" + type_name + "' is not defined",
                      function.return_type()->location());
    }
  } else if (auto multi_return = dynamic_cast<func::MultipleReturnType*>(function.return_type().get())) {
    // Multiple return types are not fully supported yet
    report_type_error("Multiple return types are not fully supported",
                  function.return_type()->location());
  } else {
      // If no return type specified, assume void
    current_return_type_ = sym_table::Type::create_void_type();
  }
  
  // Check parameters
  for (const auto& param : function.parameters()) {
    param->accept(*this);
  }
  
  // Check function body
  if (function.body()) {
    function.body()->accept(*this);
  }
  
  // Restore previous return type
  current_return_type_ = prev_return_type;
}

void TypeChecker::visit(func::Parameter& param) {
  // Verify parameter type exists
  std::string type_name = param.type()->name();
  auto param_type = symbol_table_->lookup_type(type_name);
  
  if (!param_type) {
    report_type_error("Parameter type '" + type_name + "' is not defined",
                    param.type()->location());
  }
}

void TypeChecker::visit(func::SingleReturnType& ret_type) {
  // Verify return type exists
  std::string type_name = ret_type.identifier()->name();
  auto return_type = symbol_table_->lookup_type(type_name);
  
  if (!return_type) {
    report_type_error("Return type '" + type_name + "' is not defined",
                    ret_type.location());
  }
}

void TypeChecker::visit(func::MultipleReturnType& ret_types) {
  // Verify all return types exist
  for (const auto& type_id : ret_types.identifiers()) {
    std::string type_name = type_id->name();
    auto return_type = symbol_table_->lookup_type(type_name);
    
    if (!return_type) {
      report_type_error("Return type '" + type_name + "' is not defined",
                    type_id->location());
    }
  }
}

void TypeChecker::visit(func::Body& body) {
  // Type check all statements in the function body
  for (const auto& stmt : body.statements()) {
    stmt->accept(*this);
  }
}

void TypeChecker::visit(func::ReturnTypeInfo&) {
  // Base class visit, nothing to do here
}

void TypeChecker::visit(ident::Identifier&) {
  // Simple identifier, nothing to check
}

void TypeChecker::visit(ident::TypeIdentifier&) {
  // Simple type identifier, nothing to check
}

void TypeChecker::visit(stmt::Statement&) {
  // Base class visit, nothing to do here
}

void TypeChecker::visit(stmt::Empty&) {
   // Empty statement, nothing to check
}

void TypeChecker::visit(stmt::Block& block) {
  for (const auto& stmt : block.statements()) {
    stmt->accept(*this);
  }
}

void TypeChecker::visit(stmt::Return& ret) {
  if (ret.expression()) {
    // Type check the expression
    (*ret.expression())->accept(*this);
    
    // Get the type of the expression
    auto expr_type = (*ret.expression())->type();
    
    // If we have a current return type, check for compatibility
    if (current_return_type_) {
      if (!is_safe_assignment(current_return_type_, expr_type)) {
        report_type_error(
            "Return value type '" + get_type_name(expr_type) + 
            "' is not compatible with function return type '" + 
            get_type_name(current_return_type_) + "'",
            ret.location());
      }
    }
  } else {
    // No expression, must be a void return
    if (current_return_type_ && current_return_type_->get_name() != "void") {
      report_type_error(
          "Empty return in function with non-void return type '" + 
          get_type_name(current_return_type_) + "'",
          ret.location());
    }
  }
}

void TypeChecker::visit(stmt::Print& print) {
    // Type check the expression
    print.expression()->accept(*this);
    
    // Any type can be printed, so no additional checks needed
}

void TypeChecker::visit(stmt::VariableDeclaration& var_decl) {
  // Check if the type exists
  auto var_type = var_decl.type();
  if (!var_type) {
    report_type_error("Variable type is undefined", var_decl.location());
    return;
  }
  
  // If there's an initializer, type check it
  if (var_decl.initialiser()) {
    auto init_expr = *var_decl.initialiser();
    init_expr->accept(*this);
    
    // Check type compatibility
    if (!is_safe_assignment(var_type, init_expr->type())) {
      report_type_error(
          "Cannot initialise variable of type '" + get_type_name(var_type) + 
          "' with expression of type '" + get_type_name(init_expr->type()) + "'",
          var_decl.location());
    }
  }
}

void TypeChecker::visit(stmt::Assignment& assign) {
    // Lookup the variable being assigned
    auto var_symbol = symbol_table_->lookup_symbol(assign.target()->name());
    if (!var_symbol) {
        report_type_error("Assignment to undefined variable '" + assign.target()->name() + "'",
                     assign.location());
        return;
    }
    
    // Check if the variable is const
    if (var_symbol->get_kind() == sym_table::SymbolKind::CONST) {
        report_type_error("Cannot assign to const variable '" + assign.target()->name() + "'",
                     assign.location());
    }
    
    // Type check the value expression
    assign.value()->accept(*this);
    
    // Check type compatibility
    auto var_type = var_symbol->get_type();
    auto expr_type = assign.value()->type();
    
    if (!is_safe_assignment(var_type, expr_type)) {
        report_type_error(
            "Cannot assign value of type '" + get_type_name(expr_type) + 
            "' to variable '" + assign.target()->name() + "' of type '" + 
            get_type_name(var_type) + "'",
            assign.location());
    }
}

void TypeChecker::visit(expr::Expression&) {
    // Base expression class, nothing to do here
}

void TypeChecker::visit(expr::Literal& lit) {
    // Set the type based on the literal variant
    lit.set_type(get_literal_type(lit.value()));
    
    // Validate string literals for UTF-8 encoding
    if (auto string_val = std::get_if<std::string>(&lit.value())) {
        if (!is_valid_utf8(*string_val)) {
            report_type_error("String literal contains invalid UTF-8 encoding", lit.location());
        }
    }
}

void TypeChecker::visit(expr::Binary& bin) {
    // Type check both operands
    bin.lhs()->accept(*this);
    bin.rhs()->accept(*this);
    
    // Get operand types
    auto lhs_type = bin.lhs()->type();
    auto rhs_type = bin.rhs()->type();
    
    // Determine result type
    auto result_type = get_result_type(bin.op(), lhs_type, rhs_type);
    
    if (!result_type) {
        // Get readable operation string
        std::string op_str;
        if (std::holds_alternative<BinaryOp>(bin.op())) {
            BinaryOp op = std::get<BinaryOp>(bin.op());
            switch (op) {
                case BinaryOp::ADD: op_str = "+"; break;
                case BinaryOp::SUB: op_str = "-"; break;
                case BinaryOp::MUL: op_str = "*"; break;
                case BinaryOp::DIV: op_str = "/"; break;
                case BinaryOp::MOD: op_str = "%"; break;
                case BinaryOp::B_AND: op_str = "&"; break;
                case BinaryOp::B_OR: op_str = "|"; break;
                case BinaryOp::L_AND: op_str = "&&"; break;
                case BinaryOp::L_OR: op_str = "||"; break;
                default: op_str = "unknown"; break;
            }
        } else if (std::holds_alternative<RelationalOp>(bin.op())) {
            RelationalOp op = std::get<RelationalOp>(bin.op());
            switch (op) {
                case RelationalOp::EQ: op_str = "=="; break;
                case RelationalOp::NEQ: op_str = "!="; break;
                case RelationalOp::LT: op_str = "<"; break;
                case RelationalOp::GT: op_str = ">"; break;
                case RelationalOp::LEQ: op_str = "<="; break;
                case RelationalOp::GEQ: op_str = ">="; break;
                default: op_str = "unknown"; break;
            }
        }
        
        report_type_error(
            "Invalid operation '" + op_str + "' between types '" + 
            get_type_name(lhs_type) + "' and '" + get_type_name(rhs_type) + "'",
            bin.location());
    } else {
        // Set the type of the binary expression
        bin.set_type(result_type);
    }
}

void TypeChecker::visit(expr::Unary& unary) {
    // Type check the operand
    unary.operand()->accept(*this);
    
    // Get operand type
    auto operand_type = unary.operand()->type();
    
    // Determine result type
    auto result_type = get_unary_result_type(unary.op(), operand_type);
    
    if (!result_type) {
        // Get readable operation string
        std::string op_str;
        switch (unary.op()) {
            case UnaryOp::NEG: op_str = "-"; break;
            case UnaryOp::B_NOT: op_str = "~"; break;
            case UnaryOp::L_NOT: op_str = "!"; break;
            default: op_str = "unknown"; break;
        }
        
        report_type_error(
            "Invalid unary operation '" + op_str + "' on type '" + 
            get_type_name(operand_type) + "'",
            unary.location());
    } else {
        // Set the type of the unary expression
        unary.set_type(result_type);
    }
}

void TypeChecker::visit(expr::Variable& var) {
    // Lookup the variable in the symbol table
    auto symbol = symbol_table_->lookup_symbol(var.identifier()->name());
    
    if (!symbol) {
        report_type_error("Undefined variable '" + var.identifier()->name() + "'",
                     var.location());
        return;
    }
    
    // Set the variable's type
    var.set_type(symbol->get_type());
}

void TypeChecker::visit(expr::FunctionCall& call) {
    // Lookup the function in the symbol table
    auto func_symbol = symbol_table_->lookup_symbol(call.function()->name());
    
    if (!func_symbol || func_symbol->get_kind() != sym_table::SymbolKind::FUNC) {
        report_type_error("Undefined function '" + call.function()->name() + "'",
                     call.location());
        return;
    }
    
    // Check argument count
    const auto& params = func_symbol->get_params();
    const auto& args = call.arguments();
    
    if (params.size() != args.size()) {
        report_type_error(
            "Function '" + call.function()->name() + "' expects " + 
            std::to_string(params.size()) + " arguments, but got " + 
            std::to_string(args.size()),
            call.location());
        return;
    }
    
    // Type check each argument
    for (size_t i = 0; i < args.size(); ++i) {
        args[i]->accept(*this);
        
        // Check argument type compatibility
        auto param_type = params[i]->get_type();
        auto arg_type = args[i]->type();
        
        if (!is_safe_assignment(param_type, arg_type)) {
            report_type_error(
                "Argument " + std::to_string(i+1) + " to function '" + 
                call.function()->name() + "' has incompatible type '" + 
                get_type_name(arg_type) + "', expected '" + 
                get_type_name(param_type) + "'",
                args[i]->location());
        }
    }
    
    // Set the function call's return type
    call.set_type(func_symbol->get_type());
}
