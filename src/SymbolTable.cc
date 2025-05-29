#include "SymbolTable.hh"

#include "SymbolTable.hh"

namespace argc {

auto StructType::add_field(const std::string& name, std::shared_ptr<Type> type) -> void {
  fields_.emplace(name, std::move(type));
}

auto StructType::get_field(const std::string& name) const -> std::shared_ptr<Type> {
  const auto it = fields_.find(name);
  return it != fields_.end() ? it->second : nullptr;
}

}  // namespace argc