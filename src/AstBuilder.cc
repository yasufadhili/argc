#include "AstBuilder.hh"

#include "Ast.hh"

using namespace argc::ast;

antlrcpp::Any AstBuilder::visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *context) {
  auto module_decl { std::make_shared<ModuleDeclaration>() };
  for (auto stmt_ctx : context -> statement() ) {
    auto result = visit(stmt_ctx);
    static_assert(std::is_same_v<decltype(result), antlrcpp::Any>, "Visit did not return antlrcpp::Any");
    module_decl->statements().emplace_back(visit(stmt_ctx).as<stmt_ptr>());
  }
  return module_decl;
}

antlrcpp::Any AstBuilder::visitStatement(ArgonParser::StatementContext *context) {
  if (context->getStart()->getText() == "return") {
    auto ret { std::make_shared<ReturnStatement>() };
    ret->expression() = visit(context->expression()).as<expr_ptr>();
    return ret;
  } else {
    auto expr_stmt { std::make_shared<ExpressionStatement>() };
    return expr_stmt;
  }
}


