#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "../ast/ast.hxx"
#include "../token/token.hxx"


namespace parser {

  using namespace token;
  using namespace ast;

  class Parser {

    std::string file_;
    std::vector<Token> tokens_;
    size_t index_{0};

    auto create_error(const std::string &msg) const -> void;
    auto create_error(const std::string &msg, const std::string &lexeme) const -> void;

    [[nodiscard]] auto is_at_end() const -> bool;

    auto peek(const size_t &offset) -> Token;
    auto advance() -> Token;

    [[nodiscard]] auto match(const TokenType &) -> bool;
    [[nodiscard]] auto match(const std::vector<TokenType> &) -> bool;
    //[[nodiscard]] auto match(const std::string &) -> bool;

    auto skip_newlines() -> void;

    auto parse_program() -> std::shared_ptr<prog::Program>;
    auto parse_module() -> std::shared_ptr<module::Module>;
    auto parse_function() -> std::shared_ptr<func::Function>;
    auto parse_statement() -> std::shared_ptr<stmt::Statement>;
    auto parse_expression() -> std::shared_ptr<expr::Expression>;

    // Stmts
    auto parse_block_stmt() -> std::shared_ptr<stmt::Block>;
    auto parse_return_stmt() -> std::shared_ptr<stmt::Return>;

    // Expr
    // Expression -> TernaryExpr
    // TernaryExpr -> LogicalOrExpr ["?" Expression ":" TernaryExpr]
    // LogicalOrExpr -> LogicalAndExpr ("||" LogicalAndExpr)*
    // LogicalAndExpr -> BitwiseOrExpr ("&&" BitwiseOrExpr)*
    // BitwiseOrExpr -> BitwiseXorExpr ("|" BitwiseXorExpr)*
    // BitwiseXorExpr -> BitwiseAndExpr ("^" BitwiseAndExpr)*
    // BitwiseAndExpr -> EqualityExpr ("&" EqualityExpr)*
    // EqualityExpr -> ComparisonExpr (("==" | "!=") ComparisonExpr)*
    // ComparisonExpr -> ShiftExpr (("<" | ">" | "<=" | ">=") ShiftExpr)*
    // ShiftExpr -> TermExpr (("<<" | ">>") TermExpr)*
    // TermExpr -> FactorExpr (("+" | "-") FactorExpr)*
    // FactorExpr -> UnaryExpr (("*" | "/") UnaryExpr)*
    // UnaryExpr -> "~" UnaryExpr | PrimaryExpr
    // PrimaryExpr -> INTEGER | "(" Expression ")"
    auto parse_ternary_expr() -> std::shared_ptr<expr::Expression>;
    //auto parse_binary_expr() -> std::shared_ptr<expr::Binary>;
    auto parse_unary_expr() -> std::shared_ptr<expr::Expression>;
    auto parse_term() -> std::shared_ptr<expr::Expression>;
    auto parse_factor() -> std::shared_ptr<expr::Expression>;
    auto parse_primary() -> std::shared_ptr<expr::Expression>;
    auto parse_literal_expr() -> std::shared_ptr<expr::Literal>;
    auto parse_comparison() -> std::shared_ptr<expr::Expression>;
    auto parse_equality() -> std::shared_ptr<expr::Expression>;
    auto parse_logical_and() -> std::shared_ptr<expr::Expression>;
    auto parse_logical_or() -> std::shared_ptr<expr::Expression>;
    auto parse_shift_expr() -> std::shared_ptr<expr::Expression>;
    auto parse_bitwise_xor() -> std::shared_ptr<expr::Expression>;
    auto parse_bitwise_or() -> std::shared_ptr<expr::Expression>;
    auto parse_bitwise_and() -> std::shared_ptr<expr::Expression>;

  public:

    Parser(std::string file, const std::vector<Token> &tokens) : file_(std::move(file)), tokens_(tokens) {}

    auto parse() -> std::shared_ptr<prog::Program>;

  };


}


