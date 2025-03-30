
#include "parser.hxx"

namespace parser {


  auto Parser::create_error(const std::string &msg) const -> void {
    fprintf(stderr, "%s at %d:%d",
            msg.c_str(),
            static_cast<int>(tokens_.at(index_).line),
            static_cast<int>(tokens_.at(index_).column)
            //tokens_.at(index_).location.filename.c_str()
    );
    exit(EXIT_FAILURE);
  }

  auto Parser::create_error(const std::string &msg, const std::string &lexeme) const -> void {
    fprintf(stderr, "%s '%s' at %d:%d",
            msg.c_str(),
            lexeme.c_str(),
            static_cast<int>(tokens_.at(index_).line),
            static_cast<int>(tokens_.at(index_).column)
            //tokens_.at(index_).location.filename.c_str()
    );
    exit(EXIT_FAILURE);
  }

  auto Parser::is_at_end() const -> bool {
    return tokens_.at(index_).type == TokenType::END_OF_FILE;
  }

  auto Parser::peek(const size_t &offset = 0) -> Token {
    const size_t idx = index_ + offset;
    if (idx >= tokens_.size()) {
      // Return EOF token if we're past the end
      return {TokenType::END_OF_FILE, "", 0, 0, 0};
    }
    return tokens_.at(idx);
  }

  auto Parser::advance() -> Token {
    if (is_at_end()) return tokens_.at(index_);
    index_++;
    return tokens_.at(index_);
  }

  auto Parser::match(const TokenType &type) -> bool {
    return peek().type == type;
  }

  auto Parser::match(const std::vector<TokenType> &types) -> bool {
    for (const auto &type: types) {
      if (peek().type == type) {
        return true;
      }
    }
    return false;
  }

  auto Parser::skip_newlines() -> void {
    while (peek().type == TokenType::SEMICOLON) advance();
  }

  auto Parser::parse_expression() -> std::shared_ptr<expr::Expression> {
    return parse_ternary_expr();
  }

  auto Parser::parse_ternary_expr() -> std::shared_ptr<expr::Expression> {
    auto condition = parse_logical_or();

    if (match(TokenType::QUESTION)) {
      advance(); // Consume '?'
      auto true_branch = parse_expression();

      if (!match(TokenType::COLON)) {
        create_error("Expected ':' in ternary expression");
      }
      advance(); // Consume ':'

      auto false_branch = parse_ternary_expr();
      return std::make_shared<expr::Ternary>(condition, true_branch, false_branch);
    }

    return condition;
  }

  auto Parser::parse_logical_or() -> std::shared_ptr<expr::Expression> {
    auto left = parse_logical_and();

    while (match(TokenType::L_OR)) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_logical_and();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_logical_and() -> std::shared_ptr<expr::Expression> {
    auto left = parse_bitwise_or();

    while (match(TokenType::L_AND)) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_bitwise_or();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_bitwise_or() -> std::shared_ptr<expr::Expression> {
    auto left = parse_bitwise_xor();

    while (match(TokenType::B_OR)) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_bitwise_xor();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_bitwise_xor() -> std::shared_ptr<expr::Expression> {
    auto left = parse_bitwise_and();

    while (match(TokenType::B_XOR)) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_bitwise_and();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_bitwise_and() -> std::shared_ptr<expr::Expression> {
    auto left = parse_equality();

    while (match(TokenType::B_AND)) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_equality();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_equality() -> std::shared_ptr<expr::Expression> {
    auto left = parse_comparison();

    while (match({TokenType::EQ, TokenType::NEQ})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_comparison();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_comparison() -> std::shared_ptr<expr::Expression> {
    auto left = parse_shift_expr();

    while (match({TokenType::LT, TokenType::GT, TokenType::LTEQ, TokenType::GTEQ})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_shift_expr();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_shift_expr() -> std::shared_ptr<expr::Expression> {
    auto left = parse_term();

    while (match({TokenType::SHL, TokenType::SHR})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_term();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_term() -> std::shared_ptr<expr::Expression> {
    auto left = parse_factor();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_factor();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_factor() -> std::shared_ptr<expr::Expression> {
    auto left = parse_unary_expr();

    while (match({TokenType::TIMES, TokenType::DIVIDE, TokenType::MODULO})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_unary_expr();
      left = std::make_shared<expr::Binary>(op, left, right);
    }

    return left;
  }

  auto Parser::parse_unary_expr() -> std::shared_ptr<expr::Expression> {
    if (match({TokenType::TILDE, TokenType::MINUS})) {
      Token op = peek();
      advance(); // Consume the operator
      auto right = parse_unary_expr();
      return std::make_shared<expr::Unary>(op, right);
    }

    return parse_primary();
  }

  auto Parser::parse_primary() -> std::shared_ptr<expr::Expression> {
    if (match(TokenType::INTEGER)) {
      return parse_literal_expr();
    }
    else if (match(TokenType::LEFT_PAREN)) {
      advance(); // Consume '('
      auto expr = parse_expression();

      if (!match(TokenType::RIGHT_PAREN)) {
        create_error("Expected ')' after expression");
      }
      advance(); // Consume ')'

      return std::make_shared<expr::Grouping>(expr);
    }

    create_error("Expected expression, got", peek().lexeme);
    return nullptr; // Unreachable, but needed for compiler
  }

  auto Parser::parse_literal_expr() -> std::shared_ptr<expr::Literal> {
    if (!match(TokenType::INTEGER)) {
      create_error("Expected integer literal");
    }
    std::string value{peek().lexeme};
    advance(); // Consume the integer
    return std::make_shared<expr::Literal>(value);
  }


  auto Parser::parse_return_stmt() -> std::shared_ptr<stmt::Return> {
    if (peek().type != TokenType::RET) {
      create_error("Expected a return statement.");
    }
    advance();
    std::shared_ptr<expr::Expression> expr {parse_expression()};
    if (peek().type != TokenType::SEMICOLON) {
      create_error("Expected ';' after return statement.");
    }
    advance();
    return std::make_shared<stmt::Return>(expr);
  }

  auto Parser::parse_var_stmt() -> std::shared_ptr<stmt::Var> {
    if (peek().type != TokenType::VAR) {
      create_error("Expected 'var' keyword");
    }
    advance();

    if (peek().type != TokenType::IDENTIFIER) {
      create_error("Expected identifier");
    }
    std::string name {peek().lexeme};
    advance();

    if (peek().type != TokenType::COLON) {
      create_error("Expected ':'");
    }
    advance();

    if (peek().type != TokenType::TYPE_IDENTIFIER) {
      create_error("Expected type identifier");
    }
    std::string type {peek().lexeme};
    advance();

    if (peek().type == TokenType::SEMICOLON) {
      return std::make_shared<stmt::Var>(name, type, nullptr);
    }

    if (peek().type != TokenType::EQ) {
      create_error("Expected type identifier");
    }
    advance();

    std::shared_ptr<expr::Expression> exp {parse_expression()};

    return std::make_shared<stmt::Var>(name, type, exp);
  }


  auto Parser::parse_block_stmt() -> std::shared_ptr<stmt::Block> {

    if (peek().type != TokenType::LEFT_BRACE) {
      create_error("Expected a '{' to start statement block.");
    }
    advance();

    std::vector<std::shared_ptr<stmt::Statement>> stmts;

    while (peek().type != TokenType::RIGHT_BRACE) {
      stmts.emplace_back(parse_statement());
    }

    if (peek().type != TokenType::RIGHT_BRACE) {
      create_error("Expected '}' to end statement block.");
    }
    advance();

    return std::make_shared<stmt::Block>(stmts);
  }

  auto Parser::parse_statement() -> std::shared_ptr<stmt::Statement> {

    if (peek().type == TokenType::RET) {
      return parse_return_stmt();
    }

    if (peek().type == TokenType::VAR) {
      return parse_var_stmt();
    }

    create_error("Expected a statement");
    return nullptr;
  }

  auto Parser::parse_function() -> std::shared_ptr<func::Function> {
    if (peek().type != TokenType::DEF) {
      create_error("Expected function");
    }
    advance();

    if (peek().type != TokenType::IDENTIFIER) {
      create_error("Expected identifier");
    }
    std::string name { peek().lexeme };
    advance();

    if (peek().type != TokenType::LEFT_PAREN) {
      create_error("Expected '(");
    }
    advance();
    if (peek().type != TokenType::RIGHT_PAREN) {
      create_error("Expected ')");
    }
    advance();

    if (peek().type != TokenType::RIGHT_ARROW) {
      create_error("Expected '->");
    }
    advance();

    if (peek().type != TokenType::IDENTIFIER) {
      create_error("Expected type identifier");
    }
    std::string rt {peek().lexeme};
    advance();

    if (peek().type != TokenType::LEFT_BRACE) {
      create_error("Expected '{");
    }

    std::shared_ptr<stmt::Block> body { parse_block_stmt() };

    return std::make_shared<func::Function>(name, rt, body);
  }

  auto Parser::parse_module() -> std::shared_ptr<module::Module> {

    if (peek().type != TokenType::MODULE) {
      create_error("Expected module.");
    }
    advance();

    if (peek().type != TokenType::IDENTIFIER) {
      create_error("Expected module identifier.");
    }
    std::string name { peek().lexeme };
    advance();

    if (peek().type != TokenType::SEMICOLON) {
      create_error("Expected ';' after module definition");
    }
    advance();

    std::vector<std::shared_ptr<func::Function>> fns;

    while (peek().type != TokenType::END_OF_FILE) {
      fns.emplace_back(parse_function());
    }

    return std::make_shared<module::Module>(name, fns);
  }

  auto Parser::parse_program() -> std::shared_ptr<prog::Program> {
    std::vector<std::shared_ptr<module::Module>> modules;
    if (peek().type == TokenType::END_OF_FILE) {
      fprintf(stderr, "Empty source file");
      exit(1);
    }
    while (peek().type != TokenType::END_OF_FILE) {
      modules.emplace_back(parse_module());
    }
    return std::make_shared<prog::Program>(modules);
  }

  auto Parser::parse() -> std::shared_ptr<prog::Program> {
    return parse_program();
  }



}




