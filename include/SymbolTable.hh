#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <chrono>
#include <atomic>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

namespace argc {
  /**
   * @brief Forward declarations
   */
  class ScopeManager;
  class SymbolCache;
  class TypeSystem;
  class ThreadSafetyGuard;
  class ModuleManager;
  class SymbolAnalyser;
  class SymbolTableDebugger;

  /**
   * @brief Represents different kinds of symbols in the Argon language
   */
  enum class SymbolKind {
    Module,
    Function,
    Variable,
    Type,
    Parameter,
    Field,
    Label,
    Alias,
    Generic,
    Interface
  };

  /**
   * @brief Represents visibility levels for symbols
   */
  enum class Visibility {
    Public,
    Protected,
    Private,
    Internal
  };

  /**
   * @brief Represents different kinds of scopes
   */
  enum class ScopeKind {
    Global,
    Module,
    Function,
    Block,
    Struct,
    Interface,
    Namespace,
    Generic
  };

  /**
   * @brief Error codes for symbol operations
   */
  enum class SymbolError {
    DuplicateSymbol,
    SymbolNotFound,
    InvalidScope,
    InvalidKind,
    VisibilityViolation,
    CyclicDependency,
    TemplateError,
    ImportError
  };

  /**
   * @brief Location information for symbols in source code
   */
  struct SourceLocation {
    std::string file;
    size_t line;
    size_t column;
    size_t offset;
  };

  /**
   * @brief Attributes associated with a symbol
   */
  struct SymbolAttributes {
    Visibility visibility = Visibility::Internal;
    bool is_mutable = true;
    bool is_static = false;
    bool is_extern = false;
    bool is_thread_local = false;
    std::optional<std::string> documentation;
    SourceLocation definition_location;
    std::vector<std::string> tags; // For custom attributes
  };

  /**
   * @brief Base class for all symbols
   */
  class Symbol {
  public:
    virtual ~Symbol() = default;

    SymbolKind kind;
    std::string name;
    SymbolAttributes attributes;

    template<typename T>
    T &as() { return static_cast<T &>(*this); }

    template<typename T>
    const T &as() const { return static_cast<const T &>(*this); }
  };

  /**
   * @brief Function symbol representation
   */
  struct FunctionSymbol : Symbol {
    std::vector<class Parameter> parameters;
    class TypeInfo return_type;
    bool is_variadic = false;
    bool is_async = false;
    bool is_generator = false;
    std::optional<std::string> export_name; // For FFI
  };

  /**
   * @brief Expected type for error handling
   */
  template<typename T, typename E>
  class Expected {
  public:
    Expected(T value) : value_(std::move(value)), has_value_(true) {
    }

    Expected(E error) : error_(std::move(error)), has_value_(false) {
    }

    bool hasValue() const { return has_value_; }
    const T &value() const { return value_; }
    const E &error() const { return error_; }
    operator bool() const { return has_value_; }

  private:
    T value_;
    E error_;
    bool has_value_;
  };

  using SymbolId = uint64_t;
  using ScopeId = uint64_t;
  using TypeId = uint64_t;
  using ModuleId = uint64_t;

  /**
   * @brief Main Symbol Table class for the Argon compiler
   * 
   * This class provides the core functionality for symbol management,
   * including scope management, symbol lookup, and type system integration.
   */
  class SymbolTable {
  public:
    SymbolTable();

    ~SymbolTable();

    // Scope Management
    auto enterScope(std::optional<std::string> scope_name = std::nullopt,
                    ScopeKind kind = ScopeKind::Block) -> ScopeId;

    auto exitScope() -> void;

    auto getCurrentScope() -> ScopeId;

    // Symbol Management
    auto addSymbol(std::string_view name,
                   SymbolKind kind,
                   class TypeInfo type,
                   SymbolAttributes attrs = {}) -> Expected<SymbolId, SymbolError>;

    auto lookupSymbol(std::string_view name,
                      uint32_t flags = 0) -> Expected<Symbol, SymbolError>;

    // Type System Integration
    auto resolveType(class TypeExpression &expr) -> Expected<class TypeInfo, class TypeError>;

    // Module Management
    auto importModule(std::string_view module_path,
                      uint32_t flags = 0) -> Expected<ModuleId, SymbolError>;

    // Template Support
    auto instantiateTemplate(SymbolId template_id,
                             std::vector<TypeId> args) -> Expected<SymbolId, SymbolError>;

    // Thread Safety
    auto acquireSharedLock() -> std::unique_ptr<class SharedLockGuard>;

    auto acquireExclusiveLock() -> std::unique_ptr<class ExclusiveLockGuard>;

    // Debug Support
    auto dumpSymbolTable() -> std::string;

    auto getSymbolLocation(SymbolId id) -> std::optional<SourceLocation>;

    // Symbol Analysis
    auto analyzeSymbol(SymbolId id) -> class SymbolAnalysis;

    auto validateSymbols() -> std::vector<SymbolError>;

  private:
    std::unique_ptr<ScopeManager> scope_manager_;
    std::unique_ptr<SymbolCache> symbol_cache_;
    std::unique_ptr<TypeSystem> type_system_;
    std::unique_ptr<ThreadSafetyGuard> thread_guard_;
    std::unique_ptr<ModuleManager> module_manager_;
    std::unique_ptr<SymbolAnalyser> symbol_analyser_;
    std::unique_ptr<SymbolTableDebugger> debugger_;
  };
}
