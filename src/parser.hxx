#pragma once

#include <utility>

#include "ast.hxx"
#include "token.hxx"


namespace parser {
  using namespace ast;
  using namespace token;


  class Parser {
    std::vector<Token> tokens_;
    size_t index_{0};

    auto create_error(const std::string &msg) const -> void;

    auto create_error(const std::string &msg, const std::string &lexeme) const -> void;

    [[nodiscard]] auto is_at_end() const -> bool;

    auto peek(const size_t &offset) -> Token;

    auto advance() -> Token;

    [[nodiscard]] auto match(const TokenType &) -> bool;

    //[[nodiscard]] auto match(const std::vector<TokenType> &) -> bool;

    [[nodiscard]] auto match(const std::string &) -> bool;

    auto program() -> std::shared_ptr<prog::Program>;

    auto function() -> std::shared_ptr<func::Function>;

    auto statement() -> std::shared_ptr<stmt::Statement>;

    auto block_stmt() -> std::shared_ptr<stmt::Block>;

    auto return_stmt() -> std::shared_ptr<stmt::Return>;

    auto exit_stmt() -> std::shared_ptr<stmt::Exit>;

  public:
    explicit Parser(std::vector<Token>);

    auto parse() -> std::shared_ptr<Node>;
  };


  inline Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {
  }


  inline auto Parser::create_error(const std::string &msg) const -> void {
    fprintf(stderr, " %s at line %d:%d in %s ",
            msg.c_str(),
            static_cast<int>(tokens_.at(index_).location.line),
            static_cast<int>(tokens_.at(index_).location.column),
            tokens_.at(index_).location.filename.c_str()
    );
    ::exit(EXIT_FAILURE);
  }

  inline auto Parser::create_error(const std::string &msg, const std::string &lexeme) const -> void {
    fprintf(stderr, " %s '%s' at line %d:%d in %s ",
            msg.c_str(),
            lexeme.c_str(),
            static_cast<int>(tokens_.at(index_).location.line),
            static_cast<int>(tokens_.at(index_).location.column),
            tokens_.at(index_).location.filename.c_str()
    );
    ::exit(EXIT_FAILURE);
  }

  inline auto Parser::is_at_end() const -> bool {
    return tokens_.at(index_).type == TokenType::END_OF_FILE;
  }

  inline auto Parser::peek(const size_t &offset = 0) -> Token {
    const size_t idx = index_ + offset;
    if (idx >= tokens_.size()) {
      // Return EOF token if we're past the end
      return Token(TokenType::END_OF_FILE, "", "", SourceLocation("", 0, 0, 0));
    }
    return tokens_.at(idx);
  }

  inline auto Parser::advance() -> Token {
    if (is_at_end()) return tokens_.at(index_);
    index_++;
    return tokens_.at(index_);
  }

  inline auto Parser::match(const TokenType &type) -> bool {
    return peek().type == type;
  }

  inline auto Parser::function() -> std::shared_ptr<func::Function> {
    // FORMAT: let <ident>: <type> = set(){}
    if (peek().type != TokenType::KEYWORD) {
      create_error("Expected start of function definition.");
    }

    if (const auto val = std::get<KeywordType>(peek().value); val != KeywordType::LET) {
      create_error("Expected 'let' keyword to declare function");
    }

    advance(); // consume 'let'

    if (!match(TokenType::IDENTIFIER)) {
      create_error("Expected function identifier");
    }
    const std::string name = peek().lexeme;
    advance(); // identifier

    if (!match(TokenType::PUNCTUATOR)) {
      create_error("Expected ':' after function identifier");
    }
    if (const PunctuatorType punctuator = std::get<PunctuatorType>(peek().value); punctuator != PunctuatorType::COLON) {
      create_error("Expected ':' after function identifier");
    }
    advance(); // ':'

    if (peek().type != TokenType::IDENTIFIER) {
      create_error("Expected type identifier");
    }
    if (const IdentifierType rt = std::get<IdentifierType>(peek().value); rt != IdentifierType::TYPE) {
      //create_error("Expected 'type identifier after function identifier");
      const std::string err_msg{peek().lexeme + " is not a valid type identifier"};
      create_error(err_msg);
    }
    std::string return_type{peek().lexeme};
    advance(); // return type

    if (peek().type != TokenType::OPERATOR) {
      create_error("Expected operator");
    }
    if (const OperatorType op = std::get<OperatorType>(peek().value); op != OperatorType::ASSIGN) {
      create_error("Expected assignment operator '='");
    }
    advance(); // '='

    if (const KeywordType kw = std::get<KeywordType>(peek().value); kw != KeywordType::SET) {
      create_error("Expected 'set' keyword");
    }
    advance(); // 'set'

    if (peek().type != TokenType::DELIMITER) {
      create_error("Expected delimiter");
    }
    if (const DelimiterType del = std::get<DelimiterType>(peek().value); del != DelimiterType::LPAREN) {
      create_error("Expected '(' delimiter");
    }
    advance(); // '('

    std::vector<std::pair<std::string, std::string> > params;
    while (peek().type != TokenType::DELIMITER) {
      advance(); // Handle parameters soon
    }
    if (const DelimiterType del = std::get<DelimiterType>(peek().value); del != DelimiterType::RPAREN) {
      create_error("Expected ')' delimiter");
    }
    advance(); //')'

    if (peek().type != TokenType::DELIMITER) {
      create_error("Expected '{' to start function definition ");
    }
    if (const DelimiterType del = std::get<DelimiterType>(peek().value); del != DelimiterType::LBRACE) {
      create_error("Expected '{' to start function definition");
    }
    advance();

    //auto body = block_stmt();

    //if (peek().type != TokenType::DELIMITER) {
    //  create_error("Expected '}' to end function definition ");
    //}
    //if (const DelimiterType del = std::get<DelimiterType>(peek().value); del != DelimiterType::RBRACE) {
    //  create_error("Expected '}' delimiter");
    //}
    //advance();

    return std::make_shared<func::Function>(name, return_type, params, nullptr);
  }

  inline auto Parser::program() -> std::shared_ptr<prog::Program> {
    std::vector<std::shared_ptr<func::Function> > functions;
    functions.emplace_back(function());
    //while (!is_at_end()) {
    //  functions.emplace_back(function());
    //}
    return std::make_shared<prog::Program>(functions);
  }

  inline auto Parser::parse() -> std::shared_ptr<Node> {
    return program();
  }
}
