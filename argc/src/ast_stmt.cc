
#include "ast.hh"
#include <string>
#include <utility>

using namespace ast;
using namespace ast::stmt;

Block::Block(std::vector<std::shared_ptr<Statement>> stmts)
  : statements_(std::move(stmts))
{

}

void Block::accept(SemanticAnalyser &) {

}

void Block::accept(CodeGenerator &) {

}

void Block::print(const int level) {
  print_indent(level);
  std::cout << "Block Statement " <<'\n';
  for (const auto &stmt : statements()) {
    stmt->print(level + 1);
  }
}

void Empty::print(const int level) {
  print_indent(level);
  std::cout << "Empty Statement " <<'\n';
}

Return::Return(std::optional<std::shared_ptr<expr::Expression> > expr)
  : expression_(std::move(expr)){

}

void Return::accept(SemanticAnalyser &an) {
  if (!an.current_function_return_type()) {
    an.add_error("return statement outside of function");
    //return;
  }
  // check if return value is provided
  if (expression().has_value()) {
    an.visit(expression().value());
  }
}

void Return::accept(CodeGenerator &) {

}

void Return::print(const int level) {
  print_indent(level);
  std::cout << "Return Statement " <<'\n';
  if (expression().has_value()) {
    expression().value()->print(level + 1);
  }
}

VariableDeclaration::VariableDeclaration (
  std::shared_ptr<ident::Identifier> id,
  std::shared_ptr<sym_table::Type> t,
  std::optional<std::shared_ptr<expr::Expression>> init
) :
  identifier_(std::move(id)),
  type_(std::move(t)),
  initialiser_(std::move(init)) {}

void VariableDeclaration::accept(SemanticAnalyser &an) {
  std::string var_name { identifier()->name() };

  // Check if already declared in current scope
  if (an.symbol_table()->is_declared_in_current_scope(var_name)) {
    std::string err = "variable " + var_name + " already declared in this scope";
    an.report_error(err, *this);
    return;
  }

  if (initialiser()) {
    auto init_expr { initialiser().value() };
    an.visit(init_expr);

    auto init_type { init_expr };
  }
}

void VariableDeclaration::accept(CodeGenerator &) {

}

void VariableDeclaration::print(const int level) {
  print_indent(level);
  std::cout << "Variable Declaration Statement " <<'\n';
  if (initialiser().has_value()) {
    initialiser().value()->print(level + 1);
  }
}

Assignment::Assignment (
  std::shared_ptr<ident::Identifier> target,
  std::shared_ptr<expr::Expression> val
) : target_(std::move(target)), value_(std::move(val)) {}

void Assignment::accept(SemanticAnalyser &an) {
  std::string target_name { target()->name() };
  //Lookup target variable
  auto target_symbol { an.symbol_table()->lookup_symbol(target_name) };
  if (!target_symbol) {
    an.add_error("Undefined variable: " + target_name);
    //an.report_error("Undefined variable: " + target_name , *this);
    return;
  }

  // Check if target is a variable
  if (target_symbol->get_kind() != sym_table::SymbolKind::VAR &&
    target_symbol->get_kind() != sym_table::SymbolKind::PARAM) 
  {
    std::string err = target_name + " is not a variable";
    an.report_error(err, *this);
    return;
  }
}

void Assignment::accept(CodeGenerator &) {

}

void Assignment::print(const int level) {
  print_indent(level);
  std::cout << "Assignment Statement \n";
  value()->print(level + 1);
}
