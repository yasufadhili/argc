#include "ast.hh"

using namespace ast;

namespace {
  auto binary_op_to_string(ast::BinaryOp op) -> std::string {
    switch (op) {
      case ast::BinaryOp::ADD: return "+";
      case ast::BinaryOp::SUB: return "-";
      case ast::BinaryOp::MUL: return "*";
      case ast::BinaryOp::DIV: return "/";
      case ast::BinaryOp::MOD: return "%";
      case ast::BinaryOp::B_AND: return "&";
      case ast::BinaryOp::B_OR: return "|";
      case ast::BinaryOp::L_AND: return "&&";
      case ast::BinaryOp::L_OR: return "||";
      default: return "??";
    }
  }

  auto relational_op_to_string(ast::RelationalOp op) -> std::string {
    switch (op) {
      case ast::RelationalOp::LT: return "<";
      case ast::RelationalOp::GT: return ">";
      case ast::RelationalOp::EQ: return "==";
      case ast::RelationalOp::LEQ: return "<=";
      case ast::RelationalOp::GEQ: return ">=";
      case ast::RelationalOp::NEQ: return "!=";
    }
    return "??";
  }

  auto unary_op_to_string(ast::UnaryOp op) -> std::string{
    switch (op) {
      case ast::UnaryOp::NEG: return "-";
      case ast::UnaryOp::B_NOT: return "~";
      case ast::UnaryOp::L_NOT: return "!";
    }
    return "??";
  }
}


void Printer::visit(unit::TranslationUnit& unit) {
  std::cout << "Translation Unit:\n";
  indent_level_ += 2;
  for (auto& module : unit.modules()) {
    module->accept(*this);
  }
  indent_level_ -= 2;
}

void Printer::visit(mod::Module& module) {
  print_indent();
  std::cout << "Module " << module.identifier()->name() << ":\n";
  
  indent_level_ += 2;
  
  for (auto& func : module.functions()) {
    func->accept(*this);
  }
  
  for (auto& stmt : module.statements()) {
    stmt->accept(*this);
  }
  
  indent_level_ -= 2;
}

void Printer::visit(func::Function& func) {
  print_indent();
  std::cout << (func.is_public() ? "public " : "") << "fn " 
            << func.name()->name() << "(";
            
  bool first = true;
  for (const auto& param : func.parameters()) {
    if (!first) std::cout << ", ";
    std::cout << param->name()->name() << ": " << param->type()->to_string();
    first = false;
  }
  
  std::cout << ") -> " << func.return_type()->to_string() << "\n";
  
  func.body()->accept(*this);
}

void Printer::visit(stmt::Block& block) {
  print_indent();
  std::cout << "{\n";
  
  indent_level_ += 2;
  for (auto& stmt : block.statements()) {
    stmt->accept(*this);
  }
  indent_level_ -= 2;
  
  print_indent();
  std::cout << "}\n";
}

void Printer::visit(stmt::Return& ret) {
  print_indent();
  std::cout << "return";
  if (ret.expression()) {
    std::cout << " ";
    (*ret.expression())->accept(*this);
  }
  std::cout << ";\n";
}

void Printer::visit(stmt::Print& print) {
  print_indent();
  std::cout << "print ";
  print.expression()->accept(*this);
  std::cout << ";\n";
}

void Printer::visit(stmt::VariableDeclaration& decl) {
  print_indent();
  std::cout << (decl.is_const() ? "const " : "var ")
            << decl.identifier()->name() 
            << ": " << decl.type()->to_string();
  
  if (decl.initialiser()) {
    std::cout << " = ";
    (*decl.initialiser())->accept(*this);
  }
  std::cout << ";\n";
}

void Printer::visit(stmt::Assignment& assign) {
  print_indent();
  std::cout << assign.target()->name() << " = ";
  assign.value()->accept(*this);
  std::cout << ";\n";
}

void Printer::visit(expr::Binary& bin) {
  std::cout << "(";
  bin.lhs()->accept(*this);
  
  std::cout << " ";
  std::visit([](auto&& op) {
    using T = std::decay_t<decltype(op)>;
    if constexpr (std::is_same_v<T, BinaryOp>) {
      std::cout << binary_op_to_string(op);
    } else if constexpr (std::is_same_v<T, RelationalOp>) {
      std::cout << relational_op_to_string(op);
    }
  }, bin.op());
  std::cout << " ";
  
  bin.rhs()->accept(*this);
  std::cout << ")";
}

void Printer::visit(expr::Unary& un) {
  std::cout << unary_op_to_string(un.op());
  std::cout << "(";
  un.operand()->accept(*this);
  std::cout << ")";
}

void Printer::visit(expr::Literal& lit) {
  std::visit([](auto&& value) {
    std::cout << value;
  }, lit.value());
}

void Printer::visit(expr::Variable& var) {
  std::cout << var.identifier()->name();
}

void Printer::visit(ident::Identifier& id) {
  std::cout << id.name();
}

void Printer::visit(ident::TypeIdentifier& id) {
  std::cout << id.name();
}

// Default implementations for base classes
void Printer::visit(stmt::Statement&) {}
void Printer::visit(stmt::Empty&) {}
void Printer::visit(expr::Expression&) {}

