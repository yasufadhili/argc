#pragma once

#include "ArgonBaseVisitor.h"
#include "SymbolTable.hh"

namespace argc {
class SymbolCollector : public ArgonBaseVisitor {
    SymbolTable symbol_table_;
public:
    std::any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *ctx) override;
    std::any visitExpressionStmt(ArgonParser::ExpressionStmtContext *ctx) override;
    std::any visitReturnStmt(ArgonParser::ReturnStmtContext *ctx) override;
    std::any visitMulDivExpr(ArgonParser::MulDivExprContext *ctx) override;
    std::any visitAddSubExpr(ArgonParser::AddSubExprContext *ctx) override;
    std::any visitIntAtom(ArgonParser::IntAtomContext *ctx) override;
    std::any visitParenExpr(ArgonParser::ParenExprContext *ctx) override;
};
}
