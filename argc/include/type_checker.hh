#pragma once

#include "ast.hh"

namespace ast {

/**
  * TypeChecker - A visitor implementation that verifies type correctness
  * throughout the AST. It performs type checking on expressions, statements,
  * function calls, and ensures proper type compatibility.
  */
class TypeChecker final : public Visitor {
  bool error_occurred_;
  std::shared_ptr<sym_table::SymbolTable> symbol_table_;
  std::shared_ptr<sym_table::Type> current_return_type_;
  config::Config config_;
  
  // Tracked types for expressions during visitation
  std::map<std::shared_ptr<expr::Expression>, std::shared_ptr<sym_table::Type>> expression_types_;
  
  // Type compatibility rules
  struct TypeRule {
    BinaryOp op;
    sym_table::Type::TypeKind lhs_kind;
    sym_table::Type::TypeKind rhs_kind;
    sym_table::Type::TypeKind result_kind;
    std::string result_type_name;
  };
  
  // Type promotion rules for binary operations
  std::vector<TypeRule> binary_type_rules_;
  
  auto is_compatible(const std::shared_ptr<sym_table::Type>& left, 
                      const std::shared_ptr<sym_table::Type>& right) -> bool;
  
  auto get_result_type(
      const std::variant<BinaryOp, RelationalOp>& op,
      const std::shared_ptr<sym_table::Type>& left,
      const std::shared_ptr<sym_table::Type>& right) -> std::shared_ptr<sym_table::Type>;
  
  auto get_unary_result_type(
      UnaryOp op,
      const std::shared_ptr<sym_table::Type>& operand) -> std::shared_ptr<sym_table::Type>;
  
  auto is_safe_assignment(
      const std::shared_ptr<sym_table::Type>& target,
      const std::shared_ptr<sym_table::Type>& source) -> bool;
  
  auto initialise_type_rules() -> void;
  
  auto report_type_error(const std::string& message, const yy::location& location,
                      const std::optional<std::string>& code_snippet = std::nullopt,
                      const std::optional<std::string>& suggestion = std::nullopt) -> void;

  // Get type name for reporting
  auto get_type_name(const std::shared_ptr<sym_table::Type>& type) -> std::string;

  // Helper methods for handling literals
  auto get_literal_type(const expr::LiteralVariant& value) -> std::shared_ptr<sym_table::Type>;
  
  bool is_valid_utf8(const std::string& str);
    
public:
  TypeChecker();
  ~TypeChecker() override = default;
  
  bool has_errors() const { return error_occurred_; }
  
  void visit(unit::TranslationUnit&) override;
  void visit(mod::Module&) override;
  void visit(func::Function&) override;
  void visit(expr::FunctionCall&) override;
  void visit(func::Parameter&) override;
  void visit(func::SingleReturnType&) override;
  void visit(func::MultipleReturnType&) override;
  void visit(func::Body&) override;
  void visit(func::ReturnTypeInfo&) override;
  void visit(ident::Identifier&) override;
  void visit(ident::TypeIdentifier&) override;
  void visit(stmt::Statement&) override;
  void visit(stmt::Empty&) override;
  void visit(stmt::Block&) override;
  void visit(stmt::Return&) override;
  void visit(stmt::Print&) override;
  void visit(stmt::VariableDeclaration&) override;
  void visit(stmt::Assignment&) override;
  void visit(expr::Expression&) override;
  void visit(expr::Literal&) override;
  void visit(expr::Binary&) override;
  void visit(expr::Unary&) override;
  void visit(expr::Variable&) override;
};

}
