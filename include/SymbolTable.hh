#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <ostream>

#include "SourceLocation.hh"

namespace argc {

  enum class SymbolKind {MODULE, FUNCTION, VARIABLE, TYPE, PARAMETER};

  enum class TypeKind { PRIMITIVE, STRUCT, ARRAY, FUNCTION };

  class Type {
  protected:
    TypeKind kind_ { TypeKind::PRIMITIVE };
    std::string name_;
  public:
    virtual ~Type() = default;
    auto name () const -> const std::string& { return name_; }
    [[nodiscard]] auto kind () const -> TypeKind { return kind_; }
  };

  class PrimitiveType final : public Type {
  public:
    explicit PrimitiveType(std::string type_name) {
      name_ = std::move(type_name);
      kind_ = TypeKind::PRIMITIVE;
    }
  };

  class ArrayType final : public Type {
    int size_ = 0;
    std::shared_ptr<Type> element_type_;
  public:
    ArrayType(std::shared_ptr<Type> element_type, const int size) {
      kind_ = TypeKind::ARRAY;
      element_type_ = std::move(element_type);
      size_ = size;
      name_ = "array<" + element_type_->name() + ">";
    }
  };

  class StructType final : public Type {
    std::unordered_map<std::string, std::shared_ptr<Type> > fields_;
  public:
    explicit StructType(std::string type_name) {
      name_ = std::move(type_name);
      kind_ = TypeKind::STRUCT;
    }
    auto add_field(const std::string& name, std::shared_ptr<Type> type) -> void;
    auto get_field(const std::string& name) const -> std::shared_ptr<Type>;
  };

  class FunctionType final : public Type {
    std::vector<std::shared_ptr<Type>> parameter_types_;
    std::shared_ptr<Type> return_type_;
  public:
    FunctionType (std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> ret ) {
      kind_ = TypeKind::FUNCTION;
      parameter_types_ = std::move(params);
      return_type_ = std::move(ret);
      name_ = "func";
    }
  };

  class TypeTable {
    std::unordered_map<std::string, std::shared_ptr<Type>> types_;
  public:
    auto insert (std::shared_ptr<Type> t) -> void {
      types_[t->name()] = t;
    }

    auto lookup (const std::string& name) const -> std::shared_ptr<Type> {
      const auto it = types_.find(name);
      return it != types_.end() ? it->second : nullptr;
    }

  };

  class SymbolEntry {
    std::string name_;
    SymbolKind kind_;
    std::shared_ptr<Type> type_;
    int scope_level_;
    bool is_defined_;
    loc::SourceLocation location_;
  public:
    SymbolEntry (
      const std::string &name,
      const SymbolKind kind,
      std::shared_ptr<Type> type,
      const int scope_level,
      const loc::SourceLocation &location
      )
      :
    name_(name),
    kind_(kind),
    type_(std::move(type)),
    scope_level_(scope_level),
    is_defined_(false),
    location_(location)
    {
    }

    auto name () -> std::string { return name_; }
    auto scope_level () const -> int { return scope_level_; }
    auto set_scope_level (const int level) -> void { scope_level_ = level; }
    auto kind () const -> SymbolKind { return kind_; }
    auto type () const -> std::shared_ptr<Type> { return type_; }
    auto location () const -> loc::SourceLocation { return location_; }
    auto is_defined () const -> bool { return is_defined_; }
    auto set_defined (const bool val) -> void { is_defined_ = val; }

  };

  class Scope {
    int level_;
    std::string name_;
    std::shared_ptr<Scope> parent_;
    std::unordered_map<std::string, std::shared_ptr<SymbolEntry>> symbols_;
  public:

    Scope(const int lvl, const std::string& scope_name, const std::shared_ptr<Scope> &parent_scope = nullptr)
        : level_(lvl), name_(scope_name), parent_(parent_scope) {}

    auto insert(const std::shared_ptr<SymbolEntry> &entry) -> bool {
      if (symbols_.contains(entry->name())) return false; // already declared in this scope
      symbols_[entry->name()] = entry;
      return true;
    }

    auto lookup(const std::string& name) const -> std::shared_ptr<SymbolEntry> {
      auto it = symbols_.find(name);
      if (it != symbols_.end()) return it->second;
      return parent_ ? parent_->lookup(name) : nullptr;
    }

    auto lookup_current(const std::string& name) const -> std::shared_ptr<SymbolEntry> {
      const auto it = symbols_.find(name);
      return it != symbols_.end() ? it->second : nullptr;
    }

    auto level () const -> int { return level_; }
    auto name () -> std::string { return name_; }
    auto parent () -> std::shared_ptr<Scope> { return parent_; }
    auto symbols() const -> const std::unordered_map<std::string, std::shared_ptr<SymbolEntry>>& { return symbols_; }
    auto full_scope_name() const -> std::string {
      if (!parent_) return name_;
      return parent_->full_scope_name() + "::" + name_;
    }

  };

  class SymbolTable {
    std::shared_ptr<Scope> current_scope_;
    int current_level_ { 0 };
    int anonymous_scope_counter_ { 0 };

  public:
    SymbolTable() {
      current_scope_ = std::make_shared<Scope>(0, "global");
    }

    auto enter_scope(const std::string& name = "") -> void {
      std::string scope_name = name.empty()
          ? "block_" + std::to_string(anonymous_scope_counter_++)
          : name;
      current_level_++;
      current_scope_ = std::make_shared<Scope>(current_level_, scope_name, current_scope_);
    }

    auto exit_scope() -> void {
      if (current_scope_->parent()) {
        current_scope_ = current_scope_->parent();
        current_level_--;
      }
    }

    auto insert(const std::shared_ptr<SymbolEntry> &entry) const -> bool {
      entry->set_scope_level(current_level_);
      return current_scope_->insert(entry);
    }

    auto lookup(const std::string& name) const -> std::shared_ptr<SymbolEntry> {
      return current_scope_->lookup(name);
    }

    auto lookup_current(const std::string& name) const -> std::shared_ptr<SymbolEntry> {
      return current_scope_->lookup_current(name);
    }

    auto current_scope_name() const -> std::string {
      return current_scope_->name();
    }

    auto dump_current_scope(std::ostream& os) const -> void {
      os << "Scope: " << current_scope_->name() << "\n";
      for (const auto& [k, v] : current_scope_->symbols()) {
        os << "  " << k << " : " << std::to_string(static_cast<int>(v->kind())) << "\n";
      }
    }

  };


}
