#include "SymbolCollector.hh"
#include "SourceLocation.hh"
#include <fmt/core.h>
#include <string>
#include <any>

using namespace argc;
using namespace err;


std::any SymbolCollector::visitModuleDeclaration(ArgonParser::ModuleDeclarationContext *ctx) {
    std::string module_name{ctx->IDENTIFIER()->getText()};

    auto module_type = std::make_shared<ModuleType>("module");

    try {
        auto module_entry = std::make_shared<SymbolEntry>(
            module_name,
            SymbolKind::MODULE,
            module_type,
            0,
            loc::SourceLocation(0, 0, "") // TODO: Extract location from context
            );

        if (!symbol_table_.insert(module_entry)) {
            // Module already declared
            loc::SourceLocation loc(0, 0, "");
            //error_reporter_.reportQuick(
            //    ErrorCode::InvalidOperation,
            //    CompileStage::SymbolCollection,
            //    ErrorSeverity::Error,
            //    loc,
            //    "Module '" + module_name + "' already declared"
            //    );
            fmt::print("Module '{}' already declared", module_name);
        } else {
            // Mark as defined since we have the declaration
            module_entry->set_defined(true);
        }

        symbol_table_.enter_scope(module_name);

        visitChildren(ctx);

        symbol_table_.exit_scope();

    } catch (const std::runtime_error& e) {
        //loc::SourceLocation loc(0, 0, "");
        //error_reporter_.reportQuick(
        //    ErrorCode::InvalidOperation,
        //    CompileStage::SymbolCollection,
        //    ErrorSeverity::Fatal,
        //    loc,
        //    ""
        //    );
        fmt::print("Error: {}", e.what());
    }

    return nullptr;
}

std::any SymbolCollector::visitExpressionStmt(ArgonParser::ExpressionStmtContext *ctx){
    return visit(ctx->expression());
}

std::any SymbolCollector::visitReturnStmt(ArgonParser::ReturnStmtContext *ctx){
    return visit(ctx->expression());
}

std::any SymbolCollector::visitAddSubExpr(ArgonParser::AddSubExprContext *ctx){
    visit(ctx->expression(0));
    visit(ctx->expression(1));
    return nullptr;
}

std::any SymbolCollector::visitMulDivExpr(ArgonParser::MulDivExprContext *ctx) {
    visit(ctx->expression(0));
    visit(ctx->expression(1));
    return nullptr;
}

std::any SymbolCollector::visitAtomExpr(ArgonParser::AtomExprContext *ctx) {
    return visit(ctx->atom());
}

std::any SymbolCollector::visitIntAtom(ArgonParser::IntAtomContext *ctx) {
    return nullptr;
}

std::any SymbolCollector::visitParenExpr(ArgonParser::ParenExprContext *ctx){
    return visit(ctx->expression());
}

std::any SymbolCollector::visitIdentifier(ArgonParser::AtomContext *ctx) {
    return nullptr; // Don't support identifier as expression for now
}


