#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <expected>
#include <memory>
#include <chrono>
#include <span>
#include "SourceLocation.hh"

namespace argc::sym {
  class SymbolTable;
  class TypeSystem;
  class ScopeManager;
  class SymbolCache;


  enum class SymbolKind {
    Function,
    Variable,
    Type,
    Parameter,
    Label
  };

  enum class ScopeKind {
    Global,
    Function,
    Block
  };

  enum class Visibility {
    Public,
    Private,
    Internal
  };

  enum class SymbolError {
    DuplicateSymbol,
    SymbolNotFound,
    InvalidScope,
    InvalidKind
  };

  enum class TypeKind {
    Primitive,
    Struct,
    Array,
    Pointer,
    Function
  };

  struct SymbolAttributes {
    Visibility visibility = Visibility::Internal;
    bool is_mutable = true;
    bool is_static = false;
    bool is_extern = false;
    std::optional<std::string> documentation;
    loc::SourceLocation definition_location;
    std::vector<std::string> tags;
  };

  struct TypeInfo;

  struct Parameter {
    std::string name;
    TypeInfo type;
    bool is_mutable = true;
  };

  struct FunctionSymbol {
    std::vector<Parameter> parameters;
    TypeInfo return_type;
    bool is_variadic = false;
    bool is_async = false;
  };

  struct Symbol {
    std::string name;
    SymbolKind kind;
    TypeInfo type;
    SymbolAttributes attributes;
    std::optional<FunctionSymbol> function_data; // Only for Function symbols
  };

  struct TypeInfo {
    TypeKind kind;
    std::string name;
    std::optional<TypeId> parent_type;
    std::vector<TypeId> generic_parameters;
    std::vector<Field> fields; // For structs
    struct ArrayTypeData {
      TypeId element_type;
      std::optional<size_t> size;
    };

    struct FunctionTypeData {
      std::vector<TypeId> parameter_types;
      TypeId return_type;
      bool is_variadic;
    };

    union {
      ArrayTypeData array_data;
      FunctionTypeData function_data;
    };
  };

  using SymbolId = uint64_t;
  using ScopeId = uint64_t;
  using TypeId = uint64_t;

  // --- Expected Type for Error Handling ---

  template<typename T, typename E>
  using Expected = std::expected<T, E>;

  // --- Type System ---

  class TypeSystem {
  public:
    auto registerType(TypeInfo type) -> TypeId;

    auto getType(TypeId id) -> const TypeInfo &;

    auto findType(std::string_view name) -> std::optional<TypeId>;

    auto isSubtypeOf(TypeId sub, TypeId super) -> bool;

    auto findCommonSupertype(TypeId a, TypeId b) -> std::optional<TypeId>;

    auto createArrayType(TypeId element_type, std::optional<size_t> size = std::nullopt) -> TypeId;

    auto createPointerType(TypeId pointed_type) -> TypeId;

    auto createFunctionType(std::span<TypeId> params, TypeId return_type, bool is_variadic = false) -> TypeId;

    auto isTypeComplete(TypeId type) -> bool;

    auto getTypeSize(TypeId type) -> std::optional<size_t>;

    auto getTypeAlignment(TypeId type) -> std::optional<size_t>;

  private:
    std::unordered_map<TypeId, TypeInfo> types_;
    TypeId next_type_id_ = 1;
  };

  // --- Scope Manager ---

  class ScopeManager {
  public:
    auto createScope(ScopeKind kind, std::optional<std::string> name = std::nullopt) -> ScopeId;

    auto enterScope(ScopeId id) -> void;

    auto exitScope() -> void;

    auto getCurrentScope() -> ScopeId;

    auto getParentScope(ScopeId id) -> std::optional<ScopeId>;

    auto getScopeName(ScopeId id) -> std::optional<std::string>;

    auto getScopeKind(ScopeId id) -> ScopeKind;

    auto getScopeSymbols(ScopeId id) -> std::vector<SymbolId>;

  private:
    struct ScopeNode {
      ScopeKind kind;
      std::optional<std::string> name;
      ScopeId parent;
      std::vector<ScopeId> children;
      std::unordered_map<std::string, SymbolId> symbols;
    };

    std::unordered_map<ScopeId, ScopeNode> scopes_;
    std::vector<ScopeId> scope_stack_;
    ScopeId next_scope_id_ = 1;
  };

  // --- Symbol Cache ---

  class SymbolCache {
  public:
    auto cacheSymbol(SymbolId id, const Symbol &symbol) -> void;

    auto getCachedSymbol(SymbolId id) -> std::optional<Symbol>;

    auto invalidateSymbol(SymbolId id) -> void;

    auto invalidateScope(ScopeId scope) -> void;

  private:
    struct CacheEntry {
      Symbol symbol;
      std::chrono::steady_clock::time_point last_access;
      uint32_t access_count;
    };

    std::unordered_map<SymbolId, CacheEntry> cache_;
  };

  // --- Symbol Table ---

  class SymbolTable {
  public:
    SymbolTable();

    ~SymbolTable();

    // Scope Management
    auto enterScope(std::optional<std::string> scope_name = std::nullopt,
                    ScopeKind kind = ScopeKind::Block) -> ScopeId;

    auto exitScope() -> void;

    auto getCurrentScope() -> ScopeId;

    auto getScopeName(ScopeId scope_id) -> std::optional<std::string>;

    auto getScopeKind(ScopeId scope_id) -> ScopeKind;

    auto getParentScope(ScopeId scope_id) -> std::optional<ScopeId>;

    // Symbol Management
    auto addSymbol(std::string_view name,
                   SymbolKind kind,
                   TypeInfo type,
                   SymbolAttributes attrs = {}) -> Expected<SymbolId, SymbolError>;

    auto lookupSymbol(std::string_view name) -> Expected<Symbol, SymbolError>;

    auto lookupSymbolInScope(ScopeId scope_id, std::string_view name) -> Expected<Symbol, SymbolError>;

    // Type System Integration
    auto registerType(TypeInfo type) -> TypeId;

    auto getType(TypeId id) -> const TypeInfo &;

    auto findType(std::string_view name) -> std::optional<TypeId>;

  private:
    std::unique_ptr<ScopeManager> scope_manager_;
    std::unique_ptr<SymbolCache> symbol_cache_;
    std::unique_ptr<TypeSystem> type_system_;
    std::unordered_map<SymbolId, Symbol> symbols_;
    SymbolId next_symbol_id_ = 1;
  };
}
