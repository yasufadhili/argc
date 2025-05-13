#include "semantic_analyser.hh"

using namespace ast;

void SemanticAnalyser::visit(func::Function& f) {
  
  if (!f.name()) {
    REPORT_ERROR("Function with null identifier encountered", f.location());
    error_occurred_ = true;
    return;
  }
  std::string func_scope_name = "func_" + f.name()->name();
  symbol_table_->enter_scope(func_scope_name);
  
  auto func_symbol = std::make_shared<sym_table::Symbol>(
    f.name()->name(),
    sym_table::SymbolKind::FUNC,
    nullptr, // Type will be set by later down
    true,  // Function declaration means it's defined
    f.is_public() ? sym_table::AccessModifier::PUBLIC : sym_table::AccessModifier::PRIVATE,
    f.location().begin.line,
    f.location().begin.column,
    f.location().begin.filename ? f.location().begin.filename->c_str() : ""
  );

  symbol_table_->add_symbol(func_symbol);

  // Visit parameters first to add them to the symbol table within the function's scope
  std::vector<std::shared_ptr<sym_table::Type>> param_types;
  for (const auto& param : f.parameters()) {
    if (!param) {
      REPORT_ERROR("Null parameter encountered in function '" + f.name()->name() + "'", f.location());
      error_occurred_ = true;
      continue;
    }
    if (!param->identifier()) {
      REPORT_ERROR("Parameter with null identifier in function '" + f.name()->name() + "'", param->location());
      error_occurred_ = true;
      continue;
    }
    // Visit parameter to add its symbol to the current scope
    param->accept(*this);
    // Type lookup and error reporting for unknown parameter types are moved to down
  }

  if (f.return_type()) {
    f.return_type()->accept(*this);
  }

  if (auto single_ret = std::dynamic_pointer_cast<func::SingleReturnType>(f.return_type())) {
    if (!single_ret->identifier()) {
         REPORT_ERROR("Function '" + f.name()->name() + "' has return type with null identifier", single_ret->location());
         error_occurred_ = true;
         current_return_type_ = nullptr;
    } else {
      auto ret_type = symbol_table_->lookup_type(single_ret->identifier()->name());
      if (!ret_type) {
        REPORT_ERROR("Unknown return type '" + single_ret->identifier()->name() + "' for function '" + f.name()->name() + "'", single_ret->location());
        error_occurred_ = true;
        current_return_type_ = nullptr; // Cannot resolve type
      } else {
        current_return_type_ = ret_type; // Set the resolved return type
         // Also update the function symbol's type in the symbol table since it was set to nullptr initially
         if (auto func_symbol = symbol_table_->lookup_symbol(f.name()->name())) {
            if (!func_symbol->get_type()) { // Only set if not already set (should be nullptr from above)
              func_symbol->set_type(ret_type);
            } else if (func_symbol->get_type() != ret_type) {
              // This case indicates an inconsistency if SymbolCollector already set the type.
              // For now, prefer the type looked up here.
              func_symbol->set_type(ret_type);
            }
         } else {
            // This should not happen
            REPORT_ERROR("Internal Error: Function symbol '" + f.name()->name() + "' not found during semantic analysis.", f.location());
            error_occurred_ = true;
         }
      }
    }
  } else if (auto multi_ret = std::dynamic_pointer_cast<func::MultipleReturnType>(f.return_type())) {
    // Handle multiple return types (currently not supported)
    REPORT_ERROR("Multiple return types are not supported yet", multi_ret->location());
    error_occurred_ = true;
    current_return_type_ = nullptr; // Cannot handle multiple types
  } else {
    // If no explicit return type is specified, assume void
    current_return_type_ = symbol_table_->lookup_type("void"); // Use symbol table to get void type
    if (!current_return_type_) {
      // Should not happen if built-in types are added, but safe check
      REPORT_ERROR("Built-in type 'void' not found in symbol table", f.location());
      error_occurred_ = true;
    }
     // Update the function symbol's type in the symbol table if it was set to nullptr by SymbolCollector
    if (auto func_symbol = symbol_table_->lookup_symbol(f.name()->name())) {
         if (!func_symbol->get_type()) { // Only set if not already set
              func_symbol->set_type(current_return_type_);
         } else if (func_symbol->get_type() != current_return_type_ && current_return_type_) {
              // Inconsistency or type mismatch
              func_symbol->set_type(current_return_type_);
         }
    } else {
        // This should not happen if SymbolCollector ran successfully
         REPORT_ERROR("Internal Error: Function symbol '" + f.name()->name() + "' not found during semantic analysis.", f.location());
         error_occurred_ = true;
    }
  }

  // 2. Validate parameters
  for (const auto& param : f.parameters()) {
    if (!param) {
      REPORT_ERROR("Null parameter node in function '" + f.name()->name() + "'", f.location());
      error_occurred_ = true;
      continue;
    }
    param->accept(*this); // Visit parameter node - this will validate its type
  }

  if (f.body()) {
    f.body()->accept(*this);
  }

  symbol_table_->exit_scope();
  current_return_type_ = nullptr;
}

void SemanticAnalyser::visit(func::Parameter& param) {
  if (!param.identifier()) {
    REPORT_ERROR("Parameter with null identifier", param.location());
    error_occurred_ = true;
    return;
  }
  // Create parameter symbol without resolving the type here.
  // Type resolution will happen in later down.
  auto param_symbol = std::make_shared<sym_table::Symbol>(
    param.identifier()->name(),
    sym_table::SymbolKind::PARAM,
    nullptr, // Type will be set by later down
    true,    // Parameters are considered defined by the function signature
    sym_table::AccessModifier::PRIVATE, // Parameters are local to the function
    param.location().begin.line,
    param.location().begin.column,
    param.location().begin.filename ? param.location().begin.filename->c_str() : ""
  );

  symbol_table_->add_symbol(param_symbol);

  // Here, we validate the parameter's declared type by looking it up.
   if (!param.type()) {
    REPORT_ERROR("Parameter '" + param.identifier()->name() + "' has a null type node.", param.location());
    error_occurred_ = true;
    // Cannot lookup type if the type node is null, exit.
    return;
   }

   // Lookup the declared type in the symbol table
  auto declared_type = symbol_table_->lookup_type(param.type()->get_name()); // Use get_name() from the attached Type object

  if (!declared_type) {
    REPORT_ERROR("Unknown type '" + param.type()->get_name() + "' for parameter '" + param.identifier()->name() + "'", param.location());
    error_occurred_ = true;
     // The parameter symbol's type remains null in the symbol table if lookup fails
  } else {
     // If type is resolved, find the parameter symbol 
     // and set its resolved type.
     if (auto param_symbol = symbol_table_->lookup_symbol_in_current_scope(param.identifier()->name())) {
         if (!param_symbol->get_type()) { // Only set if not already set (should be nullptr from SymbolCollector)
             param_symbol->set_type(declared_type);
         } else if (param_symbol->get_type() != declared_type) {
             // This case indicates an inconsistency if SymbolCollector already set the type.
             // Prefer the type looked up here.
             param_symbol->set_type(declared_type);
         }
     } else {
         // This should not happen if SymbolCollector ran correctly and we are in the function scope
         REPORT_ERROR("Internal Error: Parameter symbol '" + param.identifier()->name() + "' not found in current scope during semantic analysis.", param.location());
         error_occurred_ = true;
     }
  }
}

void SemanticAnalyser::visit(func::ReturnTypeInfo&) {
  // Base class, no specific semantic/type logic here
}
void SemanticAnalyser::visit(func::MultipleReturnType&) {
  // Semantic/Type logic for MultipleReturnType is handled in func::Function visitor
}
void SemanticAnalyser::visit(func::SingleReturnType&) {
  // Semantic/Type logic for SingleReturnType is handled in func::Function visitor
}