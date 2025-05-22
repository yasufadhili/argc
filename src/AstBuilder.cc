#include "AstBuilder.hh"
#include "Ast.hh"
#include "fmt/chrono.h"

using namespace argc::ast;

antlrcpp::Any AstBuilder::visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *context) {
  auto module_decl { std::make_shared<ModuleDeclaration>() };
  for (auto stmt_ctx : context -> statement() ) {
    module_decl->statements().emplace_back(std::any_cast<stmt_ptr>(visit(stmt_ctx)));
  }
  return module_decl;
}

antlrcpp::Any AstBuilder::visitStatement(ArgonParser::StatementContext *context) {
  if (context->getStart()->getText() == "return") {
    auto ret { std::make_shared<ReturnStatement>() };
    ret->expression() = std::any_cast<expr_ptr>(context->expression());
    return ret;
  } else {
    auto expr_stmt { std::make_shared<ExpressionStatement>() };
    expr_stmt->expression() = std::any_cast<expr_ptr>(context->expression()) ;
    return expr_stmt;
  }
}

antlrcpp::Any AstBuilder::visitExpression(ArgonParser::ExpressionContext *context) {
  if (!context->INTEGER()) {
    fmt::print(stderr, "Only integer expression supported for now");
    return 0;
  }
  return context->INTEGER();
}



