#include <utility>

#include "symbols.hh"

namespace sym {
  Type::Type(const TypeKind kind, std::string name) : kind(kind), name(std::move(name)) {

  }

  auto Type::get_name() const -> std::string {
    return name;
  }

  auto Type::get_kind() const -> TypeKind {
    return kind;
  }

  auto Type::to_string() const -> std::string {
    switch (kind) {
      case TypeKind::PRIMITIVE:
        return "Primitive";
      default:
        return "Unknown";
    }
  }

  Symbol::Symbol(std::string name, std::shared_ptr<Type> type, const SymbolKind kind)
    : name(std::move(name)), type(std::move(type)), kind(kind)
  {
  }

  auto Symbol::get_kind() const -> SymbolKind {
    return kind;
  }

  auto Symbol::get_name() const -> std::string {
    return name;
  }

  auto Symbol::get_type() const -> std::shared_ptr<Type> {
    return type;
  }


  VariableSymbol::VariableSymbol(const std::string &name, std::shared_ptr<Type> type)
    : Symbol(name, std::move(type), SymbolKind::VARIABLE)
  {

  }

  Scope::Scope(std::string name, std::shared_ptr<Scope> parent)
    : name(std::move(name)), parent(std::move(parent)){

  }

  auto Scope::add_symbol(const std::shared_ptr<Symbol>& symbol) -> bool {
    const std::string& name { symbol->get_name() };

    if (symbols.contains(name)) {
      return false; // Symbol Already defined in this scope
    }

    symbols[name] = symbol;
    return true;
  }

  auto Scope::lookup_symbol_in_scope(const std::string &name) const -> std::shared_ptr<Symbol> {
    auto it = symbols.find(name);
    if (it == symbols.end()) {
      return it->second;
    }
    return nullptr;
  }

  auto Scope::lookup_symbol(const std::string &name) const -> std::shared_ptr<Symbol> {
    std::shared_ptr<Symbol> symbol { lookup_symbol_in_scope(name) };
    if (symbol || !parent) return symbol;
    return parent->lookup_symbol(name); // If not found in this scope, look in parent
  }

  auto Scope::create_nested_scope(const std::string &name) -> std::shared_ptr<Scope> {
    auto nested_scope { std::make_shared<Scope>(name, shared_from_this()) };
    nested_scopes.push_back(nested_scope);
    return nested_scope;
  }

  auto Scope::get_name() const -> std::string {
    return name;
  }

  auto Scope::get_parent() const -> std::shared_ptr<Scope> {
    return parent;
  }

  auto Scope::print_scope(int level) const -> void {
    const std::string ident(level * 2, ' ');
    std::cout << ident << "Scope: " << name << std::endl;
    for (const auto& [sym_name, symbol] : symbols) {
      std::cout << ident << "Symbol: " << sym_name;
      if (symbol->get_type()) {
        std::cout << ", : " << symbol->get_type()->to_string();
      }
      std::cout << std::endl;
    }

    for (const auto& nested_scope: nested_scopes) {
      nested_scope->print_scope(level + 1);
    }
  }


  SymbolTable::SymbolTable() {
    global_scope = std::make_shared<Scope>("global");
    current_scope = global_scope;

    // Add predefined types to the global scope
    add_predefined_types();
  }


  auto SymbolTable::add_predefined_types() -> void {
    const auto int_type { std::make_shared<Type>(Type::TypeKind::PRIMITIVE, "int") };

    predefined_types["int"] = int_type;
  }


  auto SymbolTable::get_predefined_type(const std::string &name) -> std::shared_ptr<Type> {
    auto it = predefined_types.find(name);
    if (it != predefined_types.end()) {
      return it->second;
    }
    return nullptr;
  }

  auto SymbolTable::enter_scope(const std::string &name) -> std::shared_ptr<Scope> {
    current_scope = current_scope->create_nested_scope(name);
    return current_scope;
  }

  auto SymbolTable::exit_scope() -> void {
    if (current_scope->get_parent()) {
      current_scope = current_scope->get_parent();
    }
  }

  auto SymbolTable::define_variable(const std::string &name, const std::shared_ptr<Type>& type) const -> bool {
    const auto sym { std::make_shared<VariableSymbol>(name, type) };
    return current_scope->add_symbol(sym);
  }

  auto SymbolTable::lookup(const std::string &name) const -> std::shared_ptr<Symbol> {
    return current_scope->lookup_symbol(name);
  }

  auto SymbolTable::get_current_scope() const -> std::shared_ptr<Scope> {
    return current_scope;
  }

  auto SymbolTable::get_global_scope() const -> std::shared_ptr<Scope> {
    return global_scope;
  }

  auto SymbolTable::print() const -> void {
    std::cout << "SymbolTable:" << std::endl;
    global_scope->print_scope();
  }




}

