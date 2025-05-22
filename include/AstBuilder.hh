#pragma once

#include "ArgonBaseVisitor.h"

namespace argc::ast {

  class AstBuilder : public ArgonBaseVisitor {
    antlrcpp::Any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *context) override;
    std::any visitStatement(ArgonParser::StatementContext *context) override;
    std::any visitExpression(ArgonParser::ExpressionContext *context) override;
  };

}
