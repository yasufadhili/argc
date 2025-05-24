#pragma once

#include "ArgonBaseVisitor.h"
#include "ArgonParser.h"
#include "antlr4-runtime.h"
#include "ErrorReporter.hh"

namespace argc::ast {

  class AstBuilder : public  ArgonBaseVisitor {

    err::ErrorReporter& error_reporter_;

    std::any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *ctx) override;

    std::any visitStatement(ArgonParser::StatementContext *context) override;

    std::any visitMulDivExpr(ArgonParser::MulDivExprContext *context) override;
    std::any visitAddSubExpr(ArgonParser::AddSubExprContext *context) override;
    std::any visitAtomExpr(ArgonParser::AtomExprContext *context) override;

    std::any visitIntAtom(ArgonParser::IntAtomContext *context) override;
    std::any visitParenExpr(ArgonParser::ParenExprContext *context) override;

    explicit AstBuilder(err::ErrorReporter er) : error_reporter_(er) {}
  };

}
