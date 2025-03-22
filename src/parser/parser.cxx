
#include "parser.hxx"

namespace parser {


  auto Parser::create_error(const std::string &msg) const -> void {
    fprintf(stderr, "%s at line %d:%d",
            msg.c_str(),
            static_cast<int>(tokens_.at(index_).line),
            static_cast<int>(tokens_.at(index_).column)
            //tokens_.at(index_).location.filename.c_str()
    );
    exit(EXIT_FAILURE);
  }

  auto Parser::create_error(const std::string &msg, const std::string &lexeme) const -> void {
    fprintf(stderr, "%s '%s' at line %d:%d",
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

  auto Parser::parse_literal_expr() -> std::shared_ptr<expr::Literal> {
    if (peek().type != TokenType::INTEGER) {
      create_error("Expected integer literal");
    }
    std::string value {peek().lexeme};
    advance();
    return std::make_shared<expr::Literal>(value);
  }


  auto Parser::parse_expression() -> std::shared_ptr<expr::Expression> {
    return parse_literal_expr();
  }

  auto Parser::parse_return_stmt() -> std::shared_ptr<stmt::Return> {
    if (peek().type != TokenType::RETURN) {
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
    if (peek().type == TokenType::RETURN) {
      return parse_return_stmt();
    }
    create_error("Expected a statement");
    return nullptr;
  }

  auto Parser::parse_function() -> std::shared_ptr<func::Function> {
    if (peek().type != TokenType::FUNCTION) {
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
      create_error("Expected ';'");
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
    while (peek().type != TokenType::END_OF_FILE) {
      modules.emplace_back(parse_module());
    }
    return std::make_shared<prog::Program>(modules);
  }

  auto Parser::parse() -> std::shared_ptr<prog::Program> {
    return parse_program();
  }



}




