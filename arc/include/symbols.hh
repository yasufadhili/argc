#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace sym {
  class Type;
  class Scope;
  class Symbol;

  class Type {
  public:
    enum class TypeKind {
      PRIMITIVE, ARRAY, STRUCT, FUNCTION, STRING, BOOL
    };

    Type(TypeKind kind, std::string name);
    virtual ~Type() = default;

    [[nodiscard]] auto get_name() const -> std::string;
    [[nodiscard]] auto get_kind() const -> TypeKind;
    [[nodiscard]] auto to_string() const -> std::string;

  private:
    TypeKind kind;
    std::string name;
  };


  class Symbol {
  public:
    enum class SymbolKind {
      VARIABLE,
      FUNCTION,
      PARAMETER,
      TYPE_DEF
    };

    Symbol(std::string  name, std::shared_ptr<Type> type, SymbolKind kind);

    virtual ~Symbol() = default;

    [[nodiscard]] auto get_name() const -> std::string;
    [[nodiscard]] auto get_kind() const -> SymbolKind;
    [[nodiscard]] auto get_type() const -> std::shared_ptr<Type>;

  protected:
    std::string name;
    std::shared_ptr<Type> type;
    SymbolKind kind;
  };

  class VariableSymbol final : public Symbol {
  public:
    VariableSymbol(const std::string& name, std::shared_ptr<Type> type);
  };


  class Scope : public std::enable_shared_from_this<Scope>{
    std::string name;
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
    std::vector<std::shared_ptr<Scope>> nested_scopes;

  public:
    explicit Scope(std::string  name, std::shared_ptr<Scope> parent = nullptr);

    auto add_symbol(const std::shared_ptr<Symbol>& symbol) -> bool;
    auto lookup_symbol_in_scope(const std::string& name) const -> std::shared_ptr<Symbol>;
    auto lookup_symbol(const std::string& name) const -> std::shared_ptr<Symbol>;
    auto create_nested_scope(const std::string& name) -> std::shared_ptr<Scope>;
    auto get_name() const -> std::string;
    auto get_parent() const -> std::shared_ptr<Scope>;
    auto print_scope(int level = 0) const -> void;
  };


  class SymbolTable {
    std::shared_ptr<Scope> global_scope;
    std::shared_ptr<Scope> current_scope;
    std::unordered_map<std::string, std::shared_ptr<Type>> predefined_types;

  public:
    SymbolTable();
    auto add_predefined_types() -> void;
    auto get_predefined_type(const std::string& name) -> std::shared_ptr<Type>;
    auto enter_scope(const std::string& name) -> std::shared_ptr<Scope>;
    auto exit_scope() -> void;
    auto define_variable(const std::string& name, const std::shared_ptr<Type>& type) const -> bool;
    auto lookup(const std::string& name) const -> std::shared_ptr<Symbol>;
    auto get_global_scope() const -> std::shared_ptr<Scope>;
    auto get_current_scope() const -> std::shared_ptr<Scope>;
    auto print() const -> void;
  };

}