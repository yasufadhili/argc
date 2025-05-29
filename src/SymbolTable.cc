#include "SymbolTable.hh"
#include <stdexcept>
#include <algorithm>

using namespace argc::sym;

auto TypeSystem::registerType(TypeInfo type) -> TypeId {
    TypeId id = next_type_id_++;
    types_.emplace(id, std::move(type));
    return id;
}

auto TypeSystem::getType(TypeId id) -> const TypeInfo& {
    auto it = types_.find(id);
    if (it == types_.end()) {
        throw std::runtime_error("Invalid TypeId");
    }
    return it->second;
}

auto TypeSystem::findType(std::string_view name) -> std::optional<TypeId> {
    for (const auto& [id, type] : types_) {
        if (type.name == name) {
            return id;
        }
    }
    return std::nullopt;
}

auto TypeSystem::isSubtypeOf(TypeId sub, TypeId super) -> bool {
    if (sub == super) {
        return true;
    }
    const auto& sub_type = getType(sub);
    if (!sub_type.parent_type) {
        return false;
    }
    return isSubtypeOf(*sub_type.parent_type, super);
}

auto TypeSystem::findCommonSupertype(TypeId a, TypeId b) -> std::optional<TypeId> {
    if (a == b) {
        return a;
    }
    const auto& type_a = getType(a);
    if (type_a.parent_type && isSubtypeOf(*type_a.parent_type, b)) {
        return b;
    }
    const auto& type_b = getType(b);
    if (type_b.parent_type && isSubtypeOf(*type_b.parent_type, a)) {
        return a;
    }
    // Simplified: only checks direct parent for now
    return std::nullopt;
}

auto TypeSystem::createArrayType(TypeId element_type, std::optional<size_t> size) -> TypeId {
    TypeInfo type;
    type.kind = TypeKind::Array;
    type.name = "array_" + getType(element_type).name;
    type.array_data = {element_type, size};
    return registerType(std::move(type));
}

auto TypeSystem::createPointerType(TypeId pointed_type) -> TypeId {
    TypeInfo type;
    type.kind = TypeKind::Pointer;
    type.name = "ptr_" + getType(pointed_type).name;
    return registerType(std::move(type));
}

auto TypeSystem::createFunctionType(std::span<TypeId> params, TypeId return_type, bool is_variadic) -> TypeId {
    TypeInfo type;
    type.kind = TypeKind::Function;
    type.name = "func";
    type.function_data = {std::vector<TypeId>(params.begin(), params.end()), return_type, is_variadic};
    return registerType(std::move(type));
}

auto TypeSystem::isTypeComplete(TypeId type) -> bool {
    const auto& type_info = getType(type);
    if (type_info.kind == TypeKind::Primitive) {
        return true;
    }
    if (type_info.kind == TypeKind::Struct) {
        return !type_info.fields.empty();
    }
    if (type_info.kind == TypeKind::Array) {
        return isTypeComplete(type_info.array_data.element_type);
    }
    return false;
}

auto TypeSystem::getTypeSize(TypeId type) -> std::optional<size_t> {
    const auto& type_info = getType(type);
    switch (type_info.kind) {
        case TypeKind::Primitive:
            if (type_info.name == "i8" || type_info.name == "u8") return 1;
            if (type_info.name == "i16" || type_info.name == "u16") return 2;
            if (type_info.name == "i32" || type_info.name == "u32" || type_info.name == "f32") return 4;
            if (type_info.name == "i64" || type_info.name == "u64" || type_info.name == "f64") return 8;
            if (type_info.name == "bool") return 1;
            return std::nullopt;
        case TypeKind::Array:
            if (type_info.array_data.size) {
                if (auto elem_size = getTypeSize(type_info.array_data.element_type)) {
                    return *elem_size * *type_info.array_data.size;
                }
            }
            return std::nullopt;
        default:
            return std::nullopt;
    }
}

auto TypeSystem::getTypeAlignment(TypeId type) -> std::optional<size_t> {
    return getTypeSize(type); // Simplified: same as size for now
}

// --- ScopeManager Implementation ---

auto ScopeManager::createScope(ScopeKind kind, std::optional<std::string> name) -> ScopeId {
    ScopeId id = next_scope_id_++;
    ScopeNode node;
    node.kind = kind;
    node.name = name;
    node.parent = scope_stack_.empty() ? 0 : scope_stack_.back();
    scopes_.emplace(id, std::move(node));
    if (node.parent != 0) {
        scopes_[node.parent].children.push_back(id);
    }
    return id;
}

auto ScopeManager::enterScope(ScopeId id) -> void {
    if (scopes_.find(id) == scopes_.end()) {
        throw std::runtime_error("Invalid ScopeId");
    }
    scope_stack_.push_back(id);
}

auto ScopeManager::exitScope() -> void {
    if (scope_stack_.empty() || scope_stack_.back() == 0) {
        throw std::runtime_error("Cannot exit global scope");
    }
    scope_stack_.pop_back();
}

auto ScopeManager::getCurrentScope() -> ScopeId {
    if (scope_stack_.empty()) {
        return 0; // Global scope
    }
    return scope_stack_.back();
}

auto ScopeManager::getParentScope(ScopeId id) -> std::optional<ScopeId> {
    auto it = scopes_.find(id);
    if (it == scopes_.end() || it->second.parent == 0) {
        return std::nullopt;
    }
    return it->second.parent;
}

auto ScopeManager::getScopeName(ScopeId id) -> std::optional<std::string> {
    auto it = scopes_.find(id);
    if (it == scopes_.end()) {
        return std::nullopt;
    }
    return it->second.name;
}

auto ScopeManager::getScopeKind(ScopeId id) -> ScopeKind {
    auto it = scopes_.find(id);
    if (it == scopes_.end()) {
        throw std::runtime_error("Invalid ScopeId");
    }
    return it->second.kind;
}

auto ScopeManager::getScopeSymbols(ScopeId id) -> std::vector<SymbolId> {
    auto it = scopes_.find(id);
    if (it == scopes_.end()) {
        return {};
    }
    std::vector<SymbolId> result;
    for (const auto& [name, symbol_id] : it->second.symbols) {
        result.push_back(symbol_id);
    }
    return result;
}

// --- SymbolCache Implementation ---

auto SymbolCache::cacheSymbol(SymbolId id, const Symbol& symbol) -> void {
    CacheEntry entry;
    entry.symbol = symbol;
    entry.last_access = std::chrono::steady_clock::now();
    entry.access_count = 1;
    cache_.emplace(id, std::move(entry));
}

auto SymbolCache::getCachedSymbol(SymbolId id) -> std::optional<Symbol> {
    auto it = cache_.find(id);
    if (it == cache_.end()) {
        return std::nullopt;
    }
    it->second.last_access = std::chrono::steady_clock::now();
    it->second.access_count++;
    return it->second.symbol;
}

auto SymbolCache::invalidateSymbol(SymbolId id) -> void {
    cache_.erase(id);
}

auto SymbolCache::invalidateScope(ScopeId scope) -> void {
    // Defer to SymbolTable for scope-based invalidation
}

// --- SymbolTable Implementation ---

SymbolTable::SymbolTable() {
    scope_manager_ = std::make_unique<ScopeManager>();
    symbol_cache_ = std::make_unique<SymbolCache>();
    type_system_ = std::make_unique<TypeSystem>();

    // Initialize global scope
    scope_manager_->createScope(ScopeKind::Global, "global");
}

SymbolTable::~SymbolTable() = default;

auto SymbolTable::enterScope(std::optional<std::string> scope_name, ScopeKind kind) -> ScopeId {
    auto scope_id = scope_manager_->createScope(kind, scope_name);
    scope_manager_->enterScope(scope_id);
    return scope_id;
}

auto SymbolTable::exitScope() -> void {
    symbol_cache_->invalidateScope(getCurrentScope());
    scope_manager_->exitScope();
}

auto SymbolTable::getCurrentScope() -> ScopeId {
    return scope_manager_->getCurrentScope();
}

auto SymbolTable::getScopeName(ScopeId scope_id) -> std::optional<std::string> {
    return scope_manager_->getScopeName(scope_id);
}

auto SymbolTable::getScopeKind(ScopeId scope_id) -> ScopeKind {
    return scope_manager_->getScopeKind(scope_id);
}

auto SymbolTable::getParentScope(ScopeId scope_id) -> std::optional<ScopeId> {
    return scope_manager_->getParentScope(scope_id);
}

auto SymbolTable::addSymbol(std::string_view name, SymbolKind kind, TypeInfo type, SymbolAttributes attrs)
    -> Expected<SymbolId, SymbolError> {
    auto scope_id = getCurrentScope();
    auto& scope = scope_manager_->scopes_.at(scope_id);
    if (scope.symbols.contains(name)) {
        return std::unexpected(SymbolError::DuplicateSymbol);
    }

    SymbolId id = next_symbol_id_++;
    Symbol symbol;
    symbol.name = std::string(name);
    symbol.kind = kind;
    symbol.type = std::move(type);
    symbol.attributes = std::move(attrs);
    if (kind == SymbolKind::Function) {
        symbol.function_data = FunctionSymbol{};
    }

    symbols_.emplace(id, symbol);
    scope.symbols.emplace(name, id);
    symbol_cache_->cacheSymbol(id, symbol);
    return id;
}

auto SymbolTable::lookupSymbol(std::string_view name) -> Expected<Symbol, SymbolError> {
    ScopeId current_scope = getCurrentScope();
    while (current_scope != 0) {
        if (auto symbol = lookupSymbolInScope(current_scope, name)) {
            return *symbol;
        }
        auto parent = getParentScope(current_scope);
        if (!parent) {
            break;
        }
        current_scope = *parent;
    }
    // Check global scope
    return lookupSymbolInScope(0, name);
}

auto SymbolTable::lookupSymbolInScope(ScopeId scope_id, std::string_view name)
    -> Expected<Symbol, SymbolError> {
    auto it = scope_manager_->scopes_.find(scope_id);
    if (it == scope_manager_->scopes_.end()) {
        return std::unexpected(SymbolError::InvalidScope);
    }
    auto symbol_it = it->second.symbols.find(name);
    if (symbol_it == it->second.symbols.end()) {
        return std::unexpected(SymbolError::SymbolNotFound);
    }
    if (auto cached = symbol_cache_->getCachedSymbol(symbol_it->second)) {
        return *cached;
    }
    auto symbol_it_global = symbols_.find(symbol_it->second);
    if (symbol_it_global == symbols_.end()) {
        return std::unexpected(SymbolError::SymbolNotFound);
    }
    symbol_cache_->cacheSymbol(symbol_it->second, symbol_it_global->second);
    return symbol_it_global->second;
}

auto SymbolTable::registerType(TypeInfo type) -> TypeId {
    return type_system_->registerType(std::move(type));
}

auto SymbolTable::getType(TypeId id) -> const TypeInfo& {
    return type_system_->getType(id);
}

auto SymbolTable::findType(std::string_view name) -> std::optional<TypeId> {
    return type_system_->findType(name);
}
