
#include "checker.hh"

using namespace checker;

auto
  checker::check_assignment_compatibility(
    const std::shared_ptr<sym::Type> &lhs,
    const std::shared_ptr<sym::Type> &rhs
  ) -> bool {

  if (!lhs || !rhs) return false;

  // Check if direct assignment is possible
  if (rhs->can_implicitly_cast_to(*lhs)) {
    return true;
  }

  return false;
}

auto
  checker::check_operator_compatibility(
    const std::shared_ptr<sym::Type> &lhs,
    const std::shared_ptr<sym::Type> &rhs,
    const std::string &op
  ) -> bool {
  if (!lhs || !rhs) return false;

  // Arithmetic operators (+, -, *, /, %)
  if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
    // Both numeric types
    if (lhs->is_numeric_type() && rhs->is_numeric_type()) {
      // % requires integral types
      if (op == "%" && (!lhs->is_integral_type() || !rhs->is_integral_type())) {
        return false;
      }
      return true;
    }

    // Pointer arithmetic
    if (op == "+" || op == "-") {
      // pointer + int, int + pointer
      if ((lhs->is_pointer_type() && rhs->is_integral_type()) ||
          (lhs->is_integral_type() && rhs->is_pointer_type())) {
        return true;
      }

      // pointer - pointer (only for -)
      if (op == "-" && lhs->is_pointer_type() && rhs->is_pointer_type()) {
        return true;
      }
    }

    return false;
  }

  // Comparison operators (==, !=, <, >, <=, >=)
  if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") {
    // Both numeric types
    if (lhs->is_numeric_type() && rhs->is_numeric_type()) {
      return true;
    }

    // Pointers can be compared
    if (lhs->is_pointer_type() && rhs->is_pointer_type()) {
      return true;
    }

    // Pointers can be compared with nullptr
    if ((lhs->is_pointer_type() && rhs->get_name() == "nullptr_t") ||
        (rhs->is_pointer_type() && lhs->get_name() == "nullptr_t")) {
      return true;
    }

    return false;
  }

  // Logical operators (&&, ||)
  if (op == "&&" || op == "||") {
    // Can use any type that can be converted to bool
    return true;
  }

  // Bitwise operators (&, |, ^, <<, >>)
  if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") {
    return lhs->is_integral_type() && rhs->is_integral_type();
  }

  return false;
}

auto
  checker::get_result_type(
    std::shared_ptr<sym::Type> lhs,
    std::shared_ptr<sym::Type> rhs,
    const std::string &op
  ) -> std::shared_ptr<sym::Type> {
  if (!lhs || !rhs) return nullptr;

  // Arithmetic operators (+, -, *, /, %)
  if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
    // Both numeric types
    if (lhs->is_numeric_type() && rhs->is_numeric_type()) {
      return sym::Type::get_common_type(*lhs, *rhs);
    }
        
    // Pointer arithmetic
    if (op == "+" || op == "-") {
      // pointer + int, int + pointer -> pointer
      if (lhs->is_pointer_type() && rhs->is_integral_type()) {
        return lhs;
      }
      if (lhs->is_integral_type() && rhs->is_pointer_type()) {
        return rhs;
      }
            
      // pointer - pointer -> int (ptrdiff_t)
      if (op == "-" && lhs->is_pointer_type() && rhs->is_pointer_type()) {
        return sym::Type::create_integer_type();
      }
    }
        
    return nullptr;
  }

  if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") {
    return sym::Type::create_bool_type();
  }

  if (op == "&&" || op == "||") {
    return sym::Type::create_bool_type();
  }

  return nullptr; // TODO Fix Dangerous
}

auto
  checker::is_callable(
    const std::shared_ptr<sym::Symbol> &symbol
  ) -> bool {
  if (!symbol) return false;

  // Check if it's a function
  if (symbol->get_kind() == sym::SymbolKind::FUNC) {
    return true;
  }

  // Check if it's a function pointer
  auto type = symbol->get_type();
  if (type && type->get_kind() == sym::Type::TypeKind::POINTER) {
    auto base_type = type->get_base_type();
    if (base_type && base_type->get_kind() == sym::Type::TypeKind::FUNCTION) {
      return true;
    }
  }

  return false;
}

auto checker::check_function_call(std::shared_ptr<sym::Symbol> function, const std::vector<std::shared_ptr<sym::Type> > &arg_types) -> bool {
  if (!function || !is_callable(function)) {
    return false;
  }

  const auto& params = function->get_params();

  // Check number of arguments
  if (params.size() != arg_types.size()) {
    // Could handle variadic functions here
    return false;
  }

  // Check each argument against corresponding parameter
  for (size_t i = 0; i < params.size(); ++i) {
    if (!arg_types[i]->can_implicitly_cast_to(*params[i]->get_type())) {
      return false;
    }
  }

  return true;
}
