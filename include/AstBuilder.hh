#pragma once

#include "ArgonBaseVisitor.h"

namespace argc::ast {

  class AstBuilder : public ArgonBaseVisitor {
    antlrcpp::Any visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *context) override;
    antlrcpp::Any visitStatement(ArgonParser::StatementContext *context) override;
  };

}
