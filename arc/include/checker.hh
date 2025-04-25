#pragma once
#include "symbols.hh"

// Type Checker
namespace checker {

auto check_assignment_compatibility(std::shared_ptr<sym::Type> lhs, std::shared_ptr<sym::Type> rhs) -> bool;
auto check_operator_compatibility(std::shared_ptr<sym::Type> lhs, std::shared_ptr<sym::Type> rhs, const std::string& op) -> bool;
auto get_result_type(std::shared_ptr<sym::Type> lhs, std::shared_ptr<sym::Type> rhs, const std::string& op) -> std::shared_ptr<sym::Type>;
auto is_callable(std::shared_ptr<sym::Symbol> symbol);
auto check_function_call(std::shared_ptr<sym::Symbol> function, const std::vector<std::shared_ptr<sym::Type>>& arg_types);

}
