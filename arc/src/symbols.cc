#include "include/symbols.hh"

using namespace sym;

std::shared_ptr<SymbolTable> SymbolTable::instance_ = nullptr;

Symbol::Symbol(std::string& name,
               SymbolKind kind,
               std::shared_ptr<Type> type,
               bool is_defined,
               AccessModifier access,
               int line,
               int column,
               const std::string& file_name)
    : name_(name), kind_(kind), type_(type), is_defined_(is_defined), is_used_(false),
      access_(access), line_(line), column_(column), filename_(file_name),
      member_symbols_(nullptr) {
}

auto Symbol::add_param(std::shared_ptr<Symbol> new_param) -> void {
  params_.push_back(new_param);
}

auto Symbol::get_params() const -> const std::vector<std::shared_ptr<Symbol> > & {
  return params_;
}

auto Symbol::set_members_symbols(std::shared_ptr<SymbolTable> new_members_symbols) -> void {
  member_symbols_ = new_members_symbols;
}

auto Symbol::get_members_symbols() const -> std::shared_ptr<SymbolTable> {
  return member_symbols_;
}

auto Symbol::print(int indent) const -> void {
  std::string indent_str(indent, ' ');

  std::cout << indent_str << "Symbol: " << name_ << std::endl;
  std::cout << indent_str << "  Kind: ";

  switch (kind_) {
    case SymbolKind::VAR: std::cout << "VARIABLE"; break;
    case SymbolKind::FUNC: std::cout << "FUNCTION"; break;
    case SymbolKind::TYPE: std::cout << "TYPE"; break;
    case SymbolKind::PARAM: std::cout << "PARAMETER"; break;
    case SymbolKind::CONST: std::cout << "CONST"; break;
    case SymbolKind::MODULE: std::cout << "MODULE"; break;
    case SymbolKind::LABEL: std::cout << "LABEL"; break;
    case SymbolKind::TEMP: std::cout << "TEMPORARY"; break;
  }
  std::cout << std::endl;

  std::cout << indent_str << "  Type: " << (type_ ? type_->to_string() : "null") << std::endl;
  std::cout << indent_str << "  Defined: " << (is_defined_ ? "yes" : "no") << std::endl;
  std::cout << indent_str << "  Used: " << (is_used_ ? "yes" : "no") << std::endl;

  if (kind_ == SymbolKind::FUNC && !params_.empty()) {
    std::cout << indent_str << "  Parameters:" << std::endl;
    for (const auto& param : params_) {
      param->print(indent + 4);
    }
  }
    
  if (member_symbols_) {
    std::cout << indent_str << "  Members:" << std::endl;
    member_symbols_->print();
  }
    
}


