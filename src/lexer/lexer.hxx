#pragma once

#include <string>
#include <stack>
#include <utility>
#include <vector>

#include "../token/token.hxx"

namespace lexer {

  class Lexer {

    std::string file_;
    std::string src_;

    size_t index_{0};
    size_t line_{1};
    size_t col_{1};

    size_t start_index_{0};
    size_t start_line_{1};
    size_t start_col_{1};

    std::vector<token::Token> tokens_;
    std::stack<char> delimiter_stack_;

    auto create_error(const std::string &) const -> void;
    auto create_error(const std::string &, const std::string &) const -> void;
    auto create_error(const std::string &, size_t, size_t) const -> void;

    [[nodiscard]] auto is_eof() const -> bool;
    [[nodiscard]] auto is_eol() const -> bool;

    [[nodiscard]] auto peek(size_t) const -> char;
    auto advance() -> char;

    auto skip_whitespace() -> void;

    auto add_token(token::TokenType, std::string &) -> void;
    void add_token(token::TokenType, const char *);
    auto scan_token() -> void;

    static auto is_single_char(char) -> bool;
    [[nodiscard]] auto is_double_char(char) const -> bool;
    auto is_keyword(const std::string &word) -> bool;
    static auto is_open_delimiter(char) -> bool;
    static auto is_close_delimiter(char) -> bool;
    static auto is_delimiter(char) -> bool;
    static auto is_operator(char) -> bool;

    auto identifier_token() -> void;
    auto delimiter_token() -> void;
    auto single_char_token() -> void;
    auto number_token() -> void;
    auto string_token() -> void;


  public:

    Lexer(std::string file, std::string src) : file_(std::move(file)), src_(std::move(src)) {}
    ~Lexer() = default;

    auto tokenise() -> std::vector<token::Token>;

  };

}

