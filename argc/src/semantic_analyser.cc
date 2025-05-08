#include "semantic_analyser.hh"
#include "error_handler.hh"
#include <variant> 
#include <string> 
#include <vector> 
#include <memory> 

using namespace ast;


// Checks if two types are compatible (e.g., for comparisons)
auto SemanticAnalyser::is_compatible(const std::shared_ptr<sym_table::Type>& left,
                              const std::shared_ptr<sym_table::Type>& right) -> bool {
  // Null check
  if (!left || !right) {
    return false;
  }

  return left->is_compatible_with(*right);
}

// Determines the resulting type of a binary or relational operation
std::shared_ptr<sym_table::Type> SemanticAnalyser::get_result_type(
  const std::variant<BinaryOp, RelationalOp>& op,
  const std::shared_ptr<sym_table::Type>& left,
  const std::shared_ptr<sym_table::Type>& right) {

  // Check for null types
  if (!left || !right) {
    return nullptr;
  }

  // Handle relational operations (always result in bool if operands are comparable)
  if (std::holds_alternative<RelationalOp>(op)) {
    // Relational ops typically require comparable types (e.g., numeric, pointers)
    // Simplified check: allow if numeric or same kind
    if ((left->is_numeric_type() && right->is_numeric_type()) ||
        left->get_kind() == right->get_kind()) { // Basic check, could be more detailed
       return symbol_table_->lookup_type("bool"); // Use symbol table to get built-in type
    }
    return nullptr; // Incomparable types
  }

  // Get the binary operation
  BinaryOp binary_op = std::get<BinaryOp>(op);

  // Special case for string concatenation with +
  if (binary_op == BinaryOp::ADD &&
    left->get_name() == "str" && right->get_name() == "str") {
    return symbol_table_->lookup_type("str"); // Use symbol table
  }

  // Handle pointer arithmetic (pointer + integral, integral + pointer, pointer - integral)
  if (binary_op == BinaryOp::ADD || binary_op == BinaryOp::SUB) {
    if (left->is_pointer_type() && right->is_integral_type()) {
      // Ensure left->get_base_type() is not null before creating pointer type
      if (left->get_base_type()) {
         return sym_table::Type::create_pointer_type(left->get_base_type());
      } else {
         // This might indicate an issue with the pointer type itself from symbol collection
         return nullptr;
      }
    }

    if (right->is_pointer_type() && left->is_integral_type() && binary_op == BinaryOp::ADD) {
      // Ensure right->get_base_type() is not null before creating pointer type
      if (right->get_base_type()) {
         return sym_table::Type::create_pointer_type(right->get_base_type());
      } else {
         // This might indicate an issue with the pointer type itself from symbol collection
         return nullptr;
      }
    }
  }

  // Handle pointer subtraction (pointer - pointer)
  if (binary_op == BinaryOp::SUB &&
    left->is_pointer_type() && right->is_pointer_type()) {
    // Pointers must have compatible base types for meaningful subtraction
    if (left->get_base_type() && right->get_base_type() && left->get_base_type()->is_compatible_with(*right->get_base_type())) {
      return symbol_table_->lookup_type("i64"); // Result of pointer difference is an integer type
    }
    return nullptr; // Incompatible pointer subtraction
  }

  // Logical operations (&&, ||)
  if (binary_op == BinaryOp::L_AND || binary_op == BinaryOp::L_OR) {
      // Operands must be convertible to bool (e.g., bool or integral)
      auto bool_type = symbol_table_->lookup_type("bool");
      if (bool_type && left->can_implicitly_cast_to(*bool_type) &&
          right->can_implicitly_cast_to(*bool_type)) {
           return bool_type;
      }
      return nullptr; // Operands not convertible to bool
  }

  // Numeric operations (+, -, *, /, %)
  if (left->is_numeric_type() && right->is_numeric_type()) {
    // Apply type promotion rules defined in sym_table::Type::get_common_type
    return sym_table::Type::get_common_type(*left, *right);
  }

  // Bitwise operations (&, |)
  if ((binary_op == BinaryOp::B_AND || binary_op == BinaryOp::B_OR) &&
    left->is_integral_type() && right->is_integral_type()) {
    // Apply type promotion rules for integral types
    return sym_table::Type::get_common_type(*left, *right);
  }

  // No compatible operation found for the given types and operator
  return nullptr;
}

// Determines the resulting type of a unary operation
std::shared_ptr<sym_table::Type> SemanticAnalyser::get_unary_result_type(
  UnaryOp op,
  const std::shared_ptr<sym_table::Type>& operand) {

  if (!operand) {
    return nullptr;
  }

  switch (op) {
    case UnaryOp::NEG: // Unary minus (-)
      // Negation requires a numeric type
      if (operand->is_numeric_type()) {
        return operand; // Result type is the same as operand type (after potential promotion, handled elsewhere)
      }
      break;

    case UnaryOp::B_NOT: // Bitwise NOT (~)
      // Bitwise NOT requires an integral type
      if (operand->is_integral_type()) {
        return operand; // Result type is the same as operand type
      }
      break;

    case UnaryOp::L_NOT: // Logical NOT (!)
      // Logical NOT works on boolean (or types convertible to bool) and produces boolean
      auto bool_type = symbol_table_->lookup_type("bool");
      if (bool_type && operand->can_implicitly_cast_to(*bool_type)) {
        return bool_type; // Result type is always boolean
      }
      break;
  }

  // No compatible operation found for the given type and operator
  return nullptr;
}

// Checks if a source type can be safely assigned to a target type (includes implicit casting)
auto SemanticAnalyser::is_safe_assignment(
  const std::shared_ptr<sym_table::Type>& target,
  const std::shared_ptr<sym_table::Type>& source) -> bool {

  if (!target || !source) {
    return false; // Cannot assign if either type is unknown
  }

  // Check if source can be implicitly cast to target type
  return source->can_implicitly_cast_to(*target);
}

// Helper to get a readable type name for error messages
std::string SemanticAnalyser::get_type_name(const std::shared_ptr<sym_table::Type>& type) {
  if (!type) {
    return "unknown"; // Handle null type pointers gracefully
  }
  return type->to_string();
}

// Helper to determine the type of a literal node
std::shared_ptr<sym_table::Type> SemanticAnalyser::get_literal_type(const expr::LiteralVariant& value) {
  // Use std::visit to handle the different possible types in the variant
  return std::visit([&](auto&& val) -> std::shared_ptr<sym_table::Type> {
    using T = std::decay_t<decltype(val)>;
    // Lookup built-in types from the symbol table
    if constexpr (std::is_same_v<T, int64_t>) {
      return symbol_table_->lookup_type("i64"); // Assume i64 for integer literals
    } else if constexpr (std::is_same_v<T, double>) {
      return symbol_table_->lookup_type("f64"); // Assume f64 for floating-point literals
    } else if constexpr (std::is_same_v<T, bool>) {
      return symbol_table_->lookup_type("bool"); // Assume bool for boolean literals
    } else if constexpr (std::is_same_v<T, u_int64_t>) {
      return symbol_table_->lookup_type("u64"); // Assume u64 for unsigned integer literals
    }
     else if constexpr (std::is_same_v<T, std::string>) { // Added string literal handling
        // Validate UTF-8 for string literals
        // This requires is_valid_utf8 function
        // For now, just return str type if available
        if (!is_valid_utf8(val)) {
             REPORT_ERROR("String literal contains invalid UTF-8 encoding", yy::location()); // Report error with dummy location
             error_occurred_ = true;
        }
        return symbol_table_->lookup_type("str"); // Assume str for string literals
    }

    else {
      // Fallback for any other unexpected types in the variant
      REPORT_ERROR("Unknown literal type encountered", yy::location()); // Report error with dummy location
      error_occurred_ = true; // Mark error
      return nullptr;
    }
  }, value);
}

auto SemanticAnalyser::is_valid_utf8(const std::string& str) -> bool {
  const unsigned char* bytes = reinterpret_cast<const unsigned char*>(str.c_str());
  const unsigned char* end = bytes + str.length();

  while (bytes < end) {
    if (bytes[0] <= 0x7F) { bytes += 1; continue; } // 1-byte sequence (ASCII)
    if ((bytes[0] & 0xE0) == 0xC0) { // 2-byte sequence
      if (bytes + 1 >= end || (bytes[1] & 0xC0) != 0x80) return false;
      bytes += 2; continue;
    }
    if ((bytes[0] & 0xF0) == 0xE0) { // 3-byte sequence
      if (bytes + 2 >= end || (bytes[1] & 0xC0) != 0x80 || (bytes[2] & 0xC0) != 0x80) return false;
      bytes += 3; continue;
    }
    if ((bytes[0] & 0xF8) == 0xF0) { // 4-byte sequence
      if (bytes + 3 >= end || (bytes[1] & 0xC0) != 0x80 ||
        (bytes[2] & 0xC0) != 0x80 || (bytes[3] & 0xC0) != 0x80) return false;
      bytes += 4; continue;
    }
    return false; // Invalid UTF-8 encoding
  }
  return true;
}

void SemanticAnalyser::visit(expr::FunctionCall& call) {
  // 1. Lookup the function symbol in the symbol table
  // SymbolCollector ensures the symbol exists if declared.
  auto func_symbol = symbol_table_->lookup_symbol(call.function()->name());

  // Check if the symbol exists and is actually a function
  if (!func_symbol || func_symbol->get_kind() != sym_table::SymbolKind::FUNC) {
    REPORT_ERROR("Call to undefined or non-function symbol '" + call.function()->name() + "'", call.location());
    error_occurred_ = true;
    call.set_type(nullptr); // Cannot determine type if function is undefined
     // Still visit arguments to find potential errors within them
    for (auto& arg : call.arguments()) {
        if (arg) arg->accept(*this);
    }
    return;
  }

  // 2. Check argument count
  const auto& params = func_symbol->get_params(); // Get parameters from the function symbol
  const auto& args = call.arguments();

  if (params.size() != args.size()) {
    REPORT_ERROR(
      "Function '" + call.function()->name() + "' expects " +
      std::to_string(params.size()) + " arguments, but received " +
      std::to_string(args.size()),
      call.location());
    error_occurred_ = true;
    // Continue to visit arguments to find potential errors within them, but type is unresolved
    call.set_type(nullptr);
  }

  // 3. Type check each argument and compare with parameter types
  size_t num_args_to_check = std::min(params.size(), args.size()); // Avoid out-of-bounds access

  for (size_t i = 0; i < num_args_to_check; ++i) {
    // Visit the argument expression first to determine its type
    if (args[i]) args[i]->accept(*this); else {
         REPORT_ERROR("Null argument node in function call to '" + call.function()->name() + "'", call.location());
         error_occurred_ = true;
         continue; // Skip type check for this null argument
    }

    // Get the types of the parameter and the argument
    // Parameter type is from the symbol table entry created by SymbolCollector
    auto param_type = params[i]->get_type();
    auto arg_type = args[i]->type(); // Argument type comes from visiting the expression

    // Ensure parameter type was resolved by SemanticAnalyser pass on Function/Parameter nodes
     if (!param_type) {
          REPORT_ERROR("Internal Error: Type for parameter '" + params[i]->get_name() + "' of function '" + func_symbol->get_name() + "' is unresolved.", call.location());
          error_occurred_ = true;
          continue; // Cannot check type if param type is unknown
     }

    // Check argument type compatibility using is_safe_assignment (allows implicit casts)
    if (!arg_type || !is_safe_assignment(param_type, arg_type)) {
      REPORT_ERROR(
        "Argument " + std::to_string(i + 1) + " to function '" +
        call.function()->name() + "' has incompatible type '" +
        get_type_name(arg_type) + "', expected '" +
        get_type_name(param_type) + "'",
        args[i]->location()); // Report error at argument location
      error_occurred_ = true;
    }
  }

   // If args.size() > params.size(), the excess arguments were already covered by the count check.
   // If params.size() > args.size(), the missing arguments were covered by the count check.

  // 4. Set the function call expression's type to the function's return type
  // Get the function's return type from the symbol table (set during visit(func::Function&))
  call.set_type(func_symbol->get_type());
}

void SemanticAnalyser::visit(func::Body& body) {
  // SemanticAnalyser assumes SymbolCollector has already entered the block scope for the function body.
  // We don't manage scopes here.

  // Visit all statements within the function body
  for (auto& stmt : body.statements()) {
    if (stmt) {
      stmt->accept(*this);
    } else {
        REPORT_ERROR("Null statement node in function body", body.location());
        error_occurred_ = true;
    }
  }

  // SemanticAnalyser assumes SymbolCollector will exit the block scope.
}

void SemanticAnalyser::visit(ident::Identifier&) {
  // Simple identifier, no specific semantic/type logic here.
  // Handled when used in expressions (expr::Variable) or statements (stmt::Assignment, func::FunctionCall).
}
void SemanticAnalyser::visit(ident::TypeIdentifier&) {
  // Simple type identifier. Type lookup happens when needed (e.g., VariableDeclaration, Parameter, Function return type).
}

void SemanticAnalyser::visit(stmt::Statement&) {
  // Base class, no specific semantic/type logic here
}
void SemanticAnalyser::visit(stmt::Empty&) {
   // Empty statement, no semantic/type logic needed
}

void SemanticAnalyser::visit(stmt::Block& block) {
  // SemanticAnalyser assumes SymbolCollector has already entered the block scope.
  // We don't manage scopes here.

  for (auto& stmt : block.statements()) {
    if (stmt) {
      stmt->accept(*this);
    } else {
        REPORT_ERROR("Null statement node in block", block.location());
        error_occurred_ = true;
    }
  }

  // SemanticAnalyser assumes SymbolCollector will exit the block scope.
}

void SemanticAnalyser::visit(stmt::Return& ret) {
  auto expr = ret.expression(); // Get the optional expression

  // Check against the current function's return type (set in visit(func::Function&))
  if (!current_return_type_ || current_return_type_->get_name() == "void") {
    // Function is declared as void or return type is unresolved
    if (expr) {
      // Error: Return statement with a value in a void function
      REPORT_ERROR("Return statement with a value in a void function", ret.location());
      error_occurred_ = true;
       // Still visit the expression to catch errors within it
      (*expr)->accept(*this);
    }
    // A simple 'return;' is valid in a void function, so no error if !expr
    return;
  }

  // Function is declared with a non-void return type
  if (!expr) {
    // Error: Missing return value in non-void function
    REPORT_ERROR("Missing return value in function with non-void return type '" + get_type_name(current_return_type_) + "'", ret.location());
    error_occurred_ = true;
    return;
  }

  // Visit the return expression to determine its type
  (*expr)->accept(*this);
  auto expr_type = (*expr)->type(); // Get type from the visited expression

  // Check if the expression's type is compatible with (can be assigned to) the function's return type
  if (!expr_type || !is_safe_assignment(current_return_type_, expr_type)) {
    REPORT_ERROR(
      "Return value type '" + get_type_name(expr_type) +
      "' is not compatible with function return type '" +
      get_type_name(current_return_type_) + "'",
      (*expr)->location()); // Report error at expression location
    error_occurred_ = true;
  }
}

void SemanticAnalyser::visit(stmt::Print& ps) {
  auto expr = ps.expression();
  if (expr) {
    // Visit the expression to determine its type
    expr->accept(*this);
    // For 'print', we might check if the type is printable, but often languages allow printing most basic types.
    // The original TypeChecker had no specific type check here, so we won't add one now.
  } else {
    // Should not happen based on grammar, but defensive check
    REPORT_ERROR("Print statement requires an expression", ps.location());
    error_occurred_ = true;
  }
}

void SemanticAnalyser::visit(stmt::VariableDeclaration& vd) {
  // Variable symbol is added by SymbolCollector.
  // Here, we validate the declared type and the initializer.

  if (!vd.identifier()) {
       REPORT_ERROR("Variable declaration with null identifier node", vd.location());
       error_occurred_ = true;
       // Cannot proceed without identifier, exit.
       return;
  }

  // Lookup the variable symbol added by SymbolCollector. It should exist.
  auto var_symbol = symbol_table_->lookup_symbol_in_current_scope(vd.identifier()->name());

  if (!var_symbol || (var_symbol->get_kind() != sym_table::SymbolKind::VAR && var_symbol->get_kind() != sym_table::SymbolKind::CONST)) {
      // This should not happen if SymbolCollector ran correctly and we are in the correct scope
      REPORT_ERROR("Internal Error: Variable symbol '" + vd.identifier()->name() + "' not found or has incorrect kind in current scope during semantic analysis.", vd.location());
      error_occurred_ = true;
      // Still visit initializer to find errors within it
      if (vd.initialiser()) {
         (*vd.initialiser())->accept(*this);
      }
      return;
  }

  // Validate and set the declared type on the symbol
  auto declared_type_node = vd.type(); // Get the AST node for the type

  if (!declared_type_node) {
       REPORT_ERROR("Variable '" + vd.identifier()->name() + "' has a null type node.", vd.location());
       error_occurred_ = true;
       // The variable symbol's type remains null in the symbol table
       // Still visit initializer
       if (vd.initialiser()) {
          (*vd.initialiser())->accept(*this);
       }
       return;
  }

  // Lookup the declared type name in the symbol table
  auto declared_type = symbol_table_->lookup_type(declared_type_node->get_name()); // Use get_name() from the Type object attached by parser

  if (!declared_type) {
      REPORT_ERROR("Unknown type '" + declared_type_node->get_name() + "' for variable '" + vd.identifier()->name() + "'", vd.location());
      error_occurred_ = true;
       // The variable symbol's type remains null in the symbol table
      // Still visit initializer
      if (vd.initialiser()) {
         (*vd.initialiser())->accept(*this);
      }
      return;
  } else {
       // Set the resolved type on the variable symbol
       if (!var_symbol->get_type()) { // Only set if not already set (should be nullptr from SymbolCollector)
            var_symbol->set_type(declared_type);
       } else if (var_symbol->get_type() != declared_type) {
           // Inconsistency or type mismatch if SymbolCollector already set the type.
           // Prefer the type looked up here.
            var_symbol->set_type(declared_type);
       }
  }

  // Check the initialiser expression if present
  if (vd.initialiser()) {
    auto initialiser_expr = *vd.initialiser();
    // Visit the initialiser expression to determine its type
    initialiser_expr->accept(*this);
    auto initialiser_type = initialiser_expr->type(); // Get type from the visited expression

    // Check if the initialiser expression's type can be safely assigned to the variable's declared type
    // Use the resolved declared_type from the symbol table
    if (!initialiser_type || !is_safe_assignment(declared_type, initialiser_type)) {
      REPORT_ERROR(
        "Type mismatch in initialiser for variable '" + (vd.identifier() ? vd.identifier()->name() : "<unknown>") +
        "': cannot assign expression of type '" + get_type_name(initialiser_type) +
        "' to variable of type '" + get_type_name(declared_type) + "'",
        initialiser_expr->location()); // Report error at initialiser location
      error_occurred_ = true;
    }
  } else {
      // If there is no initializer, check if the type is default constructible or requires initialization
      // This is a more advanced semantic check, leaving it out for now to match original scope.
  }

  // The symbol for the variable should have been created and added to the table
  // by the SymbolCollector. We don't add it here.
}

void SemanticAnalyser::visit(stmt::Assignment& assign) {
   if (!assign.target()) {
       REPORT_ERROR("Assignment statement with null target identifier", assign.location());
       error_occurred_ = true;
       // Still visit value to find errors
       if (assign.value()) assign.value()->accept(*this);
       return;
   }

  // 1. Lookup the target variable/identifier in the symbol table
  // SymbolCollector ensures the symbol exists if declared.
  auto target_symbol = symbol_table_->lookup_symbol(assign.target()->name());

  // Check if the symbol exists
  if (!target_symbol) {
    REPORT_ERROR("Assignment to undeclared variable '" + assign.target()->name() + "'", assign.location());
    error_occurred_ = true;
    // Still visit the value expression to find errors within it
    if (assign.value()) assign.value()->accept(*this);
    return;
  }

  // Check if the target is assignable (e.g., not a constant, function, module, etc.)
  // Only VAR and PARAM are assignable in this so far, CONST is not.
  switch(target_symbol->get_kind()) {
      case sym_table::SymbolKind::VAR:
      case sym_table::SymbolKind::PARAM:
          // Assignable kinds
          break;
      case sym_table::SymbolKind::CONST:
          REPORT_ERROR("Cannot assign to constant variable '" + assign.target()->name() + "'", assign.location());
          error_occurred_ = true;
           // Still visit the value expression
          if (assign.value()) assign.value()->accept(*this);
          return; // Exit if not assignable
      default:
          REPORT_ERROR("Assignment target '" + assign.target()->name() + "' is not a variable, parameter, or assignable entity", assign.location());
          error_occurred_ = true;
           // Still visit the value expression
          if (assign.value()) assign.value()->accept(*this);
          return; // Exit if not assignable
  }


  // 2. Visit the assigned value expression to determine its type
  if (!assign.value()) {
       REPORT_ERROR("Assignment statement with null value expression", assign.location());
       error_occurred_ = true;
       return; // Cannot type check without value
  }
  assign.value()->accept(*this);
  auto value_type = assign.value()->type(); // Get type from the visited expression

  // 3. Get the type of the target variable from the symbol table
  auto target_type = target_symbol->get_type();

  // Ensure target type was resolved during semantic analysis pass on declaration/parameter nodes
  if (!target_type) {
      REPORT_ERROR("Internal Error: Type for symbol '" + target_symbol->get_name() + "' is unresolved.", assign.location());
      error_occurred_ = true;
      return; // Cannot type check if target type is unknown
  }


  // 4. Check if the value's type can be safely assigned to the target's type
  if (!value_type || !is_safe_assignment(target_type, value_type)) {
    REPORT_ERROR(
      "Type mismatch in assignment to variable '" + assign.target()->name() +
      "': cannot assign value of type '" + get_type_name(value_type) +
      "' to target of type '" + get_type_name(target_type) + "'",
      assign.value()->location()); // Report error at value expression location
    error_occurred_ = true;
  }

  // No type to set on the assignment statement itself
}

void SemanticAnalyser::visit(expr::Expression&) {
  // Base expression class, no specific semantic/type logic here.
  // Derived classes handle setting their specific types after visiting children.
}

void SemanticAnalyser::visit(expr::Literal& lit) {
  // Determine and set the type of the literal based on its value
  auto literal_type = get_literal_type(lit.value());
  if (literal_type) {
    lit.set_type(literal_type);
  } else {
    // get_literal_type already reports an error, just ensure type is null
    lit.set_type(nullptr);
    // error_occurred_ is set by get_literal_type
  }
}

void SemanticAnalyser::visit(expr::Binary& bin) {
  // 1. Visit both operands first to determine their types
  if (bin.lhs()) bin.lhs()->accept(*this); else { REPORT_ERROR("Null left-hand side in binary expression", bin.location()); error_occurred_ = true; }
  if (bin.rhs()) bin.rhs()->accept(*this); else { REPORT_ERROR("Null right-hand side in binary expression", bin.location()); error_occurred_ = true; }

  // 2. Get the types of the operands (from the visited expression nodes)
  auto lhs_type = bin.lhs() ? bin.lhs()->type() : nullptr;
  auto rhs_type = bin.rhs() ? bin.rhs()->type() : nullptr;

  // If either operand's type is unknown (due to previous errors or null nodes), we cannot determine the result type
  if (!lhs_type || !rhs_type) {
    bin.set_type(nullptr);
    return;
  }

  // 3. Determine the expected result type based on the operator and operand types
  auto result_type = get_result_type(bin.op(), lhs_type, rhs_type);

  // 4. Check if a valid operation exists for these types and operator
  if (!result_type) {
    // Get readable operation string for the error message
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
        case BinaryOp::NONE: op_str = "none"; break; // Should not happen in AST
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
      }
    } else {
        op_str = "unknown"; // Should not happen
    }

    // Report type error for the invalid operation
    REPORT_ERROR(
      "Invalid operation '" + op_str + "' between types '" +
      get_type_name(lhs_type) + "' and '" + get_type_name(rhs_type) + "'",
      bin.location());
    error_occurred_ = true;
    bin.set_type(nullptr); // Cannot determine type due to error
  } else {
    // 5. Set the resulting type of the binary expression
    bin.set_type(result_type);
  }
}

void SemanticAnalyser::visit(expr::Unary& un) {

  // 1. Visit the operand first to determine its type
  if (un.operand()) un.operand()->accept(*this); else { REPORT_ERROR("Null operand in unary expression", un.location()); error_occurred_ = true; }

  // 2. Get the type of the operand (from the visited expression node)
  auto operand_type = un.operand() ? un.operand()->type() : nullptr;

   // If operand type is unknown, we cannot determine the result type
  if (!operand_type) {
      un.set_type(nullptr);
      return;
  }

  // 3. Determine the expected result type based on the operator and operand type
  auto result_type = get_unary_result_type(un.op(), operand_type);

  // 4. Check if a valid operation exists for this type and operator
  if (!result_type) {
    // Get readable operation string for the error message
    std::string op_str;
    switch (un.op()) {
      case UnaryOp::NEG: op_str = "-"; break;
      case UnaryOp::B_NOT: op_str = "~"; break;
      case UnaryOp::L_NOT: op_str = "!"; break;
    }

    // Report type error for the invalid operation
    REPORT_ERROR(
      "Invalid unary operation '" + op_str + "' on type '" +
      get_type_name(operand_type) + "'",
      un.location());
        error_occurred_ = true;
    un.set_type(nullptr); // Cannot determine type due to error
  } else {
    // 5. Set the resulting type of the unary expression
    un.set_type(result_type);
  }
}

void SemanticAnalyser::visit(expr::Variable& var) {
   if (!var.identifier()) {
       REPORT_ERROR("Variable expression with null identifier node", var.location());
       error_occurred_ = true;
       var.set_type(nullptr);
       return;
   }
  // 1. Lookup the variable symbol in the symbol table
  // SymbolCollector ensures the symbol exists if declared.
  auto symbol = symbol_table_->lookup_symbol(var.identifier()->name());

  // 2. Check if the symbol exists
  if (!symbol) {
    // This error should ideally be caught by SymbolCollector, but we re-check here
    // for robustness or cases where SymbolCollector might have missed something.
    // However, the previous SymbolCollector refactor already reports this.
    // Let's assume SymbolCollector is the primary source for this error now.
    // Keeping the check but maybe changing the error message or making it an internal error.
    // For now, keep the existing user-facing error.
    REPORT_ERROR("Use of undeclared variable '" + var.identifier()->name() + "'", var.location());
    error_occurred_ = true;
    var.set_type(nullptr); // Cannot determine type if symbol is undefined
    return;
  }

  // 3. Check if the symbol is a variable, parameter, or constant (something with a type that can be used in an expression)
  // Exclude functions, modules, types, labels, temps here - usage might be different or an error
  switch(symbol->get_kind()) {
    case sym_table::SymbolKind::VAR:
    case sym_table::SymbolKind::PARAM:
    case sym_table::SymbolKind::CONST:
    case sym_table::SymbolKind::TEMP: // If temporary symbols are used in expressions
        // These kinds are expected to have a type and be usable as an expression
        break;
    case sym_table::SymbolKind::FUNC:
         // Using a function name directly as a variable (e.g., `x = myFunc;`) might not be allowed
         REPORT_ERROR("Usage of function '" + var.identifier()->name() + "' as a variable is not allowed", var.location());
         error_occurred_ = true;
         var.set_type(nullptr);
         return;
    case sym_table::SymbolKind::TYPE:
         // Using a type name directly as a variable (e.g., `x = int;`) is not allowed
         REPORT_ERROR("Usage of type name '" + var.identifier()->name() + "' as a variable is not allowed", var.location());
         error_occurred_ = true;
         var.set_type(nullptr);
         return;
    case sym_table::SymbolKind::MODULE:
         // Using a module name directly as a variable is not allowed
         REPORT_ERROR("Usage of module name '" + var.identifier()->name() + "' as a variable is not allowed", var.location());
         error_occurred_ = true;
         var.set_type(nullptr);
         return;
    case sym_table::SymbolKind::LABEL:
         // Usage of label as a variable is not allowed
          REPORT_ERROR("Usage of label '" + var.identifier()->name() + "' as a variable is not allowed", var.location());
          error_occurred_ = true;
          var.set_type(nullptr);
          return;
    default:
        // Catch any other unexpected kinds
        REPORT_ERROR("Usage of symbol '" + var.identifier()->name() + "' as a variable is not allowed", var.location());
        error_occurred_ = true;
        var.set_type(nullptr); // Invalid usage means type is unknown
        return; // Exit if usage is invalid
  }


  // 4. Set the expression's type to the type of the found symbol
  // The symbol's type should have been resolved and set during the SemanticAnalyser pass on declarations/parameters.
  auto symbol_type = symbol->get_type();
   if (!symbol_type) {
       // This case indicates that the type resolution for this symbol failed during the
       // SemanticAnalyser pass on its declaration (VariableDeclaration or Parameter).
       REPORT_ERROR("Internal Error: Variable '" + var.identifier()->name() + "' has an unresolved type.", var.location());
       error_occurred_ = true;
   }
  var.set_type(symbol_type);

  // Mark symbol as used - handled by symbol_table_->lookup_symbol
}