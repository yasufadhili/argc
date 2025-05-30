#pragma once

#include "ArgonBaseVisitor.h"
#include "SymbolTable.hh"
#include "ErrorReporter.hh"

namespace argc {
class SymbolCollector : public ArgonBaseVisitor {
  SymbolTable symbol_table_;
  err::ErrorReporter& error_reporter_;

public:

  explicit SymbolCollector(err::ErrorReporter& reporter)
    : error_reporter_(reporter) {}

  std::any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *ctx) override;

  std::any visitExpressionStmt(ArgonParser::ExpressionStmtContext *ctx) override;
  std::any visitReturnStmt(ArgonParser::ReturnStmtContext *ctx) override;

  std::any visitAddSubExpr(ArgonParser::AddSubExprContext *ctx) override;
  std::any visitMulDivExpr(ArgonParser::MulDivExprContext *ctx) override;
  std::any visitAtomExpr(ArgonParser::AtomExprContext *ctx) override;

  std::any visitIntAtom(ArgonParser::IntAtomContext *ctx) override;
  std::any visitParenExpr(ArgonParser::ParenExprContext *ctx) override;

  std::any visitIdentifier(ArgonParser::AtomContext *ctx);

  auto getSymbolTable () -> SymbolTable& {
    return symbol_table_;
  }

private:
  auto extract_source_location(antlr4::ParserRuleContext* ctx) -> loc::SourceLocation;
  auto create_primitive_type(const std::string& type_name) -> std::shared_ptr<Type>;
};
}
