#include "AstBuilder.hh"

#include "Ast.hh"

using namespace argc::ast;

std::any AstBuilder::visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *ctx) {
  auto module_node = std::make_unique<ModuleDeclaration>();
  for (auto stmt_ctx : ctx->statement()) {
    if (stmt_ctx) {
      std::any stmt_node_any = visitStatement(stmt_ctx);
      if (stmt_node_any.has_value()) {
        try {
          // We expect visitStatement to return std::unique_ptr<Statement>
          auto stmt_node = std::any_cast<std::unique_ptr<Statement>>(stmt_node_any);
          if (stmt_node) {
            module_node->add_statement(std::move(stmt_node));
          }
        }catch (const std::bad_any_cast& e) {
          // TODO: Handle error or log if the type is not as expected
          // Throwing for now
          fmt::print(stderr, "Error casting statement node: {} \n", e.what());
        }
      }
    }
  }
  return std::any_cast<ModuleDeclaration>(*module_node);
}
