#pragma once

#include "ArgonBaseVisitor.h"
#include "ArgonParser.h"
#include "antlr4-runtime.h"

namespace argc::ast {

  class AstBuilder : public ArgonBaseVisitor {
    antlrcpp::Any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *context) override;
    antlrcpp::Any visitStatement(ArgonParser::StatementContext *context) override;
    antlrcpp::Any visitExpression(ArgonParser::ExpressionContext *context) override;
  };

}
