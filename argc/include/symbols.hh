#pragma once
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <unordered_map>

namespace sym {


class SymbolTable;


class Type {

public:
  enum class TypeKind {
    PRIMITIVE,
    ARRAY,
    STRUCT,
    UNION,
    FUNCTION,
    ENUM,
    POINTER,
    REFERENCE,
    TYPEDEF,
    AUTO,
    VOID
  };
private:
  TypeKind kind_;
  std::string name_;

  // For Composite types
  std::shared_ptr<Type> base_type_;

  // For arrays
  int array_size_;

  // For custom types like structs
  std::shared_ptr<SymbolTable> member_types_;
public:
  Type(TypeKind kind, std::string  name);

  auto get_kind() const -> TypeKind { return kind_; }
  auto get_name() const -> std::string { return name_; }

  // For composite types
  auto set_base_type(std::shared_ptr<Type> base_type) -> void { base_type_ = base_type; }
  auto get_base_type() const -> std::shared_ptr<Type> { return base_type_; }

  // For arrays
  auto set_array_size (size_t array_size) -> void { array_size_ = array_size; }
  auto get_array_size () const -> size_t { return array_size_; }

  // For structs and maybe classes in future
  auto set_member_types (std::shared_ptr<SymbolTable> member_types) -> void { member_types_ = member_types; }
  auto get_member_types () const -> std::shared_ptr<SymbolTable> { return member_types_; }

  // Type Checking Methods
  virtual bool is_compatible_with (const Type& other) const;
  virtual bool can_implicitly_cast_to (const Type& target) const;

  // Common type operations
  static auto get_common_type (const Type& t1, const Type& t2) -> std::shared_ptr<Type>;

  // Utility
  virtual auto to_string() const -> std::string;
  auto is_integral_type() const -> bool;
  auto is_floating_point_type() const -> bool;
  auto is_numeric_type() const -> bool;
  auto is_pointer_type() const -> bool;

  // Factory methods for common types
  static auto create_integer_type() -> std::shared_ptr<Type>;
  static auto create_floating_point_type() -> std::shared_ptr<Type>;
  static auto create_char_type() -> std::shared_ptr<Type>;
  static auto create_void_type() -> std::shared_ptr<Type>;
  static auto create_bool_type() -> std::shared_ptr<Type>;
  static auto create_string_type() -> std::shared_ptr<Type>;
  static auto create_pointer_type(const std::shared_ptr<Type>& base_type) -> std::shared_ptr<Type>;
  static auto create_array_type(const std::shared_ptr<Type>& element_type, int size) -> std::shared_ptr<Type>;

};


enum class SymbolKind {
  VAR,
  FUNC,
  TYPE,
  PARAM,
  CONST,
  MODULE,
  LABEL,
  TEMP
};

enum class AccessModifier {
  PUBLIC, PRIVATE, PROTECTED, DEFAULT
};


class Symbol {
  std::string name_;
  SymbolKind kind_;
  std::shared_ptr<Type> type_;
  bool is_defined_;
  bool is_used_;
  AccessModifier access_;
  int line_;
  int column_;
  std::string filename_;

  // For Functions
  std::vector<std::shared_ptr<Symbol>> params_;

  // For complex data type (structs)
  std::shared_ptr<SymbolTable> member_symbols_;

public:
  Symbol(std::string  name, SymbolKind kind, std::shared_ptr<Type> type, bool is_defined = false, AccessModifier access = AccessModifier::DEFAULT, int line = 0, int column = 0, std::string  filename = "");

  auto get_name() const -> std::string { return name_; }
  auto get_kind() const -> SymbolKind { return kind_; }
  auto get_type() const -> std::shared_ptr<Type> { return type_; }
  auto get_access() const -> AccessModifier { return access_; }
  auto get_is_defined() const -> bool { return is_defined_; }
  auto get_is_used() const -> bool { return is_used_; }
  auto get_line() const -> int { return line_; }
  auto get_column() const -> int { return column_; }
  auto get_filename() const -> std::string { return filename_; }

  auto set_defined(const bool defined) -> void { is_defined_ = defined; }
  auto set_used(const bool used) -> void { is_used_ = used; }
  auto set_type(std::shared_ptr<Type> new_type) -> void { type_ = std::move(new_type); }
  auto set_access(const AccessModifier new_access) -> void { access_ = new_access; }

  // Function Specific
  auto add_param(const std::shared_ptr<Symbol>& new_param) -> void;
  auto get_params() const -> const std::vector<std::shared_ptr<Symbol>> &;

  // Complex Type Methods
  auto set_members_symbols(const std::shared_ptr<SymbolTable> &new_members_symbols) -> void;
  auto get_members_symbols() const -> std::shared_ptr<SymbolTable>;

  auto print(int indent) const -> void;
};


class Scope {
  std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;
  std::string scope_name_;
  int scope_level_;

public:
  Scope (std::string  name = "", int level = 0);

  // Symbol management
  auto add_symbol (const std::shared_ptr<Symbol>& symbol) -> bool;
  auto lookup_symbol (const std::string& name) const -> std::shared_ptr<Symbol>;
  auto remove_symbol (const std::string& name) -> bool;

  // Getters
  auto get_scope_level () const -> int { return scope_level_; }
  auto get_scope_name () const -> std::string;

  // Iterate over symbols
  auto begin () const -> std::unordered_map<std::string, std::shared_ptr<Symbol>>::const_iterator {
    return symbols_.begin();
  }
  auto end () const -> std::unordered_map<std::string, std::shared_ptr<Symbol>>::const_iterator {
    return symbols_.end();
  }

  // Debug
  auto print() const -> void;
};


// Symbol Table class that maintains the symbol tables for different scopes
class SymbolTable {
  std::vector<std::shared_ptr<Scope>> scopes_;
  int current_scope_index_;

  // Singletom instance
  static std::shared_ptr<SymbolTable> instance_;

public:
  SymbolTable ();

  // Scope management
  auto enter_scope (const std::string& scope_name = "") -> void;
  auto exit_scope () -> void;
  auto get_current_scope () const -> std::shared_ptr<Scope>;

  // Symbol Operations
  auto add_symbol (const std::shared_ptr<Symbol>& symbol) const -> bool;
  auto lookup_symbol (const std::string& name) const -> std::shared_ptr<Symbol>;
  auto lookup_symbol_in_current_scope (const std::string& name) const -> std::shared_ptr<Symbol>;

  // Check if identifier is already defined in current scope
  auto is_declared_in_current_scope (const std::string& name) const -> bool;

  // Type Management
  auto add_type (const std::string& name, const std::shared_ptr<Type>& type) const -> bool;
  auto lookup_type (const std::string& name) const -> std::shared_ptr<Type>;

  // singleton access
  static auto get_instance () -> std::shared_ptr<SymbolTable>;

  // Debug
  auto print () const -> void;

  // Error reporting utility
  static auto report_error (const std::string& message) -> void;
  static auto report_warning (const std::string& message) -> void;

  auto check_unused_symbols () const -> void;


};


class SemanticError : public std::runtime_error {
  int line_;
  int column_;
  std::string filename_;

public:
  SemanticError(const std::string& message, int line = 0, int column = 0, const std::string& fileName = "")
        : std::runtime_error(message), line_(line), column_(column), filename_(fileName) {}

  auto get_filename () const -> const std::string { return filename_; }
  auto get_line () const -> int { return line_; }
  auto get_column () const -> int { return column_; }

};


}

