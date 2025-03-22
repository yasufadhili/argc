#pragma once

#include <memory>
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

    auto parse_program() -> std::shared_ptr<prog::Program>;
    auto parse_module() -> std::shared_ptr<module::Module>;
    auto parse_function() -> std::shared_ptr<func::Function>;
    auto parse_statement() -> std::shared_ptr<stmt::Statement>;
    auto parse_expression() -> std::shared_ptr<expr::Expression>;

    // Stmts
    auto parse_block_stmt() -> std::shared_ptr<stmt::Block>;
    auto parse_return_stmt() -> std::shared_ptr<stmt::Return>;

    // Expr
    auto parse_literal_expr() -> std::shared_ptr<expr::Literal>;

  public:

    Parser(const std::string &file, const std::vector<Token> &tokens) : file_(file), tokens_(tokens) {}

    auto parse() -> std::shared_ptr<prog::Program>;

  };


}


