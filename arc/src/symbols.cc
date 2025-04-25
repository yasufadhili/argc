#include "include/symbols.hh"
#include <utility>

#include "include/symbols.hh"

using namespace sym;

std::shared_ptr<SymbolTable> SymbolTable::instance_ = nullptr;

//=============================================================================
// Symbol Implementation
//=============================================================================

Symbol::Symbol(std::string name,
               const SymbolKind kind,
               std::shared_ptr<Type> type,
               const bool is_defined,
               const AccessModifier access,
               const int line,
               const int column,
               std::string filename)
  : name_(std::move(name)), kind_(kind), type_(std::move(type)), is_defined_(is_defined), is_used_(false),
    access_(access), line_(line), column_(column), filename_(std::move(filename)),
    member_symbols_(nullptr) {
}

auto Symbol::add_param(const std::shared_ptr<Symbol>& new_param) -> void {
  params_.push_back(new_param);
}

auto Symbol::get_params() const -> const std::vector<std::shared_ptr<Symbol> > & {
  return params_;
}

auto Symbol::set_members_symbols(const std::shared_ptr<SymbolTable> &new_members_symbols) -> void {
  member_symbols_ = new_members_symbols;
}

auto Symbol::get_members_symbols() const -> std::shared_ptr<SymbolTable> {
  return member_symbols_;
}

auto Symbol::print(const int indent) const -> void {
  const std::string indent_str(indent, ' ');

  std::cout << indent_str << "Symbol: " << name_ << std::endl;
  std::cout << indent_str << "  Kind: ";

  switch (kind_) {
    case SymbolKind::VAR: std::cout << "VARIABLE";
      break;
    case SymbolKind::FUNC: std::cout << "FUNCTION";
      break;
    case SymbolKind::TYPE: std::cout << "TYPE";
      break;
    case SymbolKind::PARAM: std::cout << "PARAMETER";
      break;
    case SymbolKind::CONST: std::cout << "CONST";
      break;
    case SymbolKind::MODULE: std::cout << "MODULE";
      break;
    case SymbolKind::LABEL: std::cout << "LABEL";
      break;
    case SymbolKind::TEMP: std::cout << "TEMPORARY";
      break;
  }
  std::cout << std::endl;

  std::cout << indent_str << "  Type: " << (type_ ? type_->to_string() : "null") << std::endl;
  std::cout << indent_str << "  Defined: " << (is_defined_ ? "yes" : "no") << std::endl;
  std::cout << indent_str << "  Used: " << (is_used_ ? "yes" : "no") << std::endl;

  if (kind_ == SymbolKind::FUNC && !params_.empty()) {
    std::cout << indent_str << "  Parameters:" << std::endl;
    for (const auto &param: params_) {
      param->print(indent + 4);
    }
  }

  if (member_symbols_) {
    std::cout << indent_str << "  Members:" << std::endl;
    member_symbols_->print();
  }
}

//=============================================================================
// Type Implementation
//=============================================================================


Type::Type(const TypeKind kind, std::string name)
  : kind_(kind), name_(std::move(name)), array_size_(-1) {
}

bool Type::is_compatible_with(const Type &other) const {
  // Same type
  if (kind_ == other.kind_ && name_ == other.name_) {
    return true;
  }

  // Numeric type compatibility
  if (is_numeric_type() && other.is_numeric_type()) {
    return true;
  }

  // Pointer compatibility
  if (is_pointer_type() && other.is_pointer_type()) {
    // Need to check base types
    if (base_type_ && other.base_type_) {
      return base_type_->is_compatible_with(*other.base_type_);
    }
  }

  // Handle void* compatibility
  if (is_pointer_type() && kind_ == TypeKind::POINTER &&
      base_type_ && base_type_->kind_ == TypeKind::VOID) {
    return other.is_pointer_type();
  }

  if (other.is_pointer_type() && other.kind_ == TypeKind::POINTER &&
      other.base_type_ && other.base_type_->kind_ == TypeKind::VOID) {
    return is_pointer_type();
  }

  return false;
}

bool Type::can_implicitly_cast_to(const Type &target) const {
  // Always can cast to the same type
  if (is_compatible_with(target)) {
    return true;
  }

  // Numeric promotions/conversions
  if (is_numeric_type() && target.is_numeric_type()) {
    // Detailed rules for numeric conversions would go here
    // Generally, smaller types can be cast to larger types
    return true;
  }

  // Pointer conversions
  if (is_pointer_type() && target.is_pointer_type()) {
    // Derived to base object conversions would be handled here

    // Null pointer constant can be converted to any pointer type
    if (kind_ == TypeKind::PRIMITIVE && name_ == "nullptr_t") {
      return true;
    }
  }

  return false;
}

auto Type::get_common_type(const Type &t1, const Type &t2) -> std::shared_ptr<Type> {
  // If types are identical, return t1's type
  if (t1.kind_ == t2.kind_ && t1.name_ == t2.name_) {
    return std::make_shared<Type>(t1.kind_, t1.name_);
  }
    
  // Handle numeric type promotions
  if (t1.is_numeric_type() && t2.is_numeric_type()) {
    // Implement numeric promotion rules
    // Float + int -> float
    if (t1.is_floating_point_type()) return std::make_shared<Type>(t1.kind_, t1.name_);
    if (t2.is_floating_point_type()) return std::make_shared<Type>(t2.kind_, t2.name_);
        
    // Both are integral types, choose the larger one
    // This is simplified; actual implementation will need detailed ranking
    if (t1.name_ == "long long" || t2.name_ == "long long") return create_integer_type(); // Simplified
    if (t1.name_ == "long" || t2.name_ == "long") return create_integer_type(); // Simplified
    return create_integer_type(); // Default
  }
    
  // Pointer arithmetic
  if (t1.is_pointer_type() && t2.is_integral_type()) {
    return std::make_shared<Type>(t1.kind_, t1.name_);
  }
  if (t2.is_pointer_type() && t1.is_integral_type()) {
    return std::make_shared<Type>(t2.kind_, t2.name_);
  }
    
  // Default - return int type as fallback
  return create_integer_type();
}

auto Type::to_string() const -> std::string {
  std::string result = name_;
    
  switch (kind_) {
    case TypeKind::POINTER:
      result = base_type_ ? base_type_->to_string() + "*" : "void*";
      break;
    case TypeKind::REFERENCE:
      result = base_type_ ? base_type_->to_string() + "&" : "unknown&";
      break;
    case TypeKind::ARRAY:
      result = base_type_ ? base_type_->to_string() + "[" + 
              (array_size_ >= 0 ? std::to_string(array_size_) : "") + "]" : "unknown[]";
      break;
    case TypeKind::FUNCTION:
      result = "function";
      break;
    default:
      // Keep the name_
      break;
  }
    
  return result;
}

auto Type::is_integral_type() const -> bool {
  if (kind_ != TypeKind::PRIMITIVE) return false;
  return name_ == "int" || name_ == "char" || name_ == "short" || 
         name_ == "long" || name_ == "long long" || name_ == "bool" ||
         name_ == "unsigned int" || name_ == "unsigned char" || 
         name_ == "unsigned short" || name_ == "unsigned long" || 
         name_ == "unsigned long long";
}

auto Type::is_floating_point_type() const -> bool {
  if (kind_ != TypeKind::PRIMITIVE) return false;
  return name_ == "float" || name_ == "double" || name_ == "long double";
}

auto Type::is_numeric_type() const -> bool {
  return is_integral_type() || is_floating_point_type();
}

auto Type::is_pointer_type() const -> bool {
  return kind_ == TypeKind::POINTER;
}

// Factory methods for common types
auto Type::create_integer_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::PRIMITIVE, "float");
}

auto Type::create_floating_point_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::PRIMITIVE, "double");
}

auto Type::create_char_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::PRIMITIVE, "char");
}

auto Type::create_string_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::PRIMITIVE, "string");
}

auto Type::create_void_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::VOID, "void");
}

auto Type::create_bool_type() -> std::shared_ptr<Type> {
  return std::make_shared<Type>(TypeKind::PRIMITIVE, "bool");
}

auto Type::create_pointer_type(const std::shared_ptr<Type>& base_type) -> std::shared_ptr<Type> {
  auto ptr_type { std::make_shared<Type>(TypeKind::POINTER, base_type->get_name() + "*") };
  ptr_type->set_base_type(base_type);
  return ptr_type;
}

auto Type::create_array_type(const std::shared_ptr<Type>& element_type, int size) -> std::shared_ptr<Type> {
  std::string arr_name = element_type->get_name() + "[" +
                          (size >= 0 ? std::to_string(size) : "") + "]";
  auto arrayType = std::make_shared<Type>(TypeKind::ARRAY, arr_name);
  arrayType->set_base_type(element_type);
  arrayType->set_array_size(size);
  return arrayType;
}

//=============================================================================
// Scope Implementation
//=============================================================================

Scope::Scope(std::string  name, const int level)
    : scope_name_(std::move(name)), scope_level_(level) {
}

auto Scope::add_symbol(const std::shared_ptr<Symbol>& symbol) -> bool {
  const std::string& name = symbol->get_name();
  // Check if symbol already exists in this scope
  if (symbols_.find(name) != symbols_.end()) {
    return false; // Symbol already exists
  }

  symbols_[name] = symbol;
  return true;
}

auto Scope::lookup_symbol(const std::string &name) const -> std::shared_ptr<Symbol> {
  auto it = symbols_.find(name);
  if (it != symbols_.end()) {
    return it->second;
  }
  return nullptr;
}

auto Scope::remove_symbol(const std::string &name) -> bool {
  //return symbols_.erase(name) > 0 ? symbols_.erase(name) : nullptr;
  return symbols_.erase(name) > 0;
}

auto Scope::get_scope_name() const -> std::string {
  return scope_name_;
}

auto Scope::print() const -> void {
  std::cout << "Scope: " << scope_name_ << " (level " << scope_level_ << ")" << std::endl;
  for (const auto& pair : symbols_) {
    pair.second->print(2);
  }
}


