#pragma once

#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <stack>
#include <map>
#include <algorithm>

#include "Registry.hxx"
#include "TokenType.hxx"
#include "Token.hxx"

class LexerError final : public std::runtime_error {

  size_t line_;
  size_t col_;
  size_t pos_;

public:

  LexerError(const std::string &msg, const size_t line, const size_t col, const size_t pos)
    : std::runtime_error(msg), line_(line), col_(col), pos_(pos) {
    throw std::runtime_error("LexerError: " + msg + " at line " + std::to_string(line_) + ":" + std::to_string(col_) + ":" + std::to_string(pos_));
  }

  [[nodiscard]] auto get_line() const -> size_t { return line_; }
  [[nodiscard]] auto get_col() const -> size_t { return col_; }
  [[nodiscard]] auto get_pos() const -> size_t { return pos_; }

  [[nodiscard]] auto what_with_location() const -> std::string {
    std::ostringstream oss;
    oss << what() << " at line " << line_ << ", column " << col_
        << " (position " << pos_ << ")";
    return oss.str();
  }

};

class Lexer {

  std::string_view src_;
  size_t line_ {1};
  size_t column_ {1};
  size_t index_ {0};
  size_t start_pos_{0};
  size_t start_line_{1};
  size_t start_column_{1};

  std::vector<Token> tokens_ {};
  bool error_occurred_ { false };
  std::vector<LexerError> errors_ {};

  std::map<char, TokenType> delimiters_map_ {
    {'{', TokenType::LBRACE},
    {'}', TokenType::RBRACE},
    {'[', TokenType::LBRACK},
    {']', TokenType::RBRACK},
    {'(', TokenType::LPAREN},
    {')', TokenType::RPAREN},
  };

  std::stack<char> delimiter_stack_;

  std::unordered_map<char, char> delimiter_pairs_ {
    {'}', '{'},
    {']', '['},
    {')', '('}
  };

  std::unordered_map<std::string, TokenType> keywords_ {
    {"exit", TokenType::EXIT},
    {"let", TokenType::LET},
    {"set", TokenType::SET}
  };

  auto reset() -> void {
    tokens_.clear();
    line_ = 1;
    column_ = 1;
    index_ = 0;
    error_occurred_ = false;
    errors_.clear();
  }

  [[nodiscard]] auto is_end() const -> bool {
    return index_ >= src_.length();
  }

  [[nodiscard]] auto peek(size_t offset = 0) const -> char {
    if (index_ + offset >= src_.length()) {
      return '\0';
    }
    return src_[index_ + offset];
  }

  auto advance() -> char {
    if (is_end()) {
      return '\0';
    }
    const char c = peek();
    index_++;
    column_++;

    if (c == '\n') {
      line_++;
      column_ = 1;
    }

    return c;
  }

  auto match(const char expected) -> bool {
    if (is_end() || peek() != expected) {
      return false;
    }

    advance();
    return true;
  }

  auto throw_error(const std::string &message) const -> void {
    throw LexerError(message, start_line_, start_column_, start_pos_);
  }

  auto add_token(TokenType type, const std::string &value) -> void {
    tokens_.emplace_back(type, value, SourceLocation(start_line_, start_column_, start_pos_));
  }

  auto add_token_with_char_info(TokenType type, const std::string &repr, char value) -> void {
    tokens_.emplace_back(type, repr, SourceLocation(start_line_, start_column_, start_pos_), value);
  }

  auto add_token_with_string_info(TokenType type, const std::string &repr, const std::string &value) -> void {
    tokens_.emplace_back(type, repr, SourceLocation(start_line_, start_column_, start_pos_), value);
  }

  auto is_open_delimiter(const char c) const -> bool {
    return c == '{' || c == '[' || c == '(';
  }

  auto is_close_delimiter(char c) -> bool {
    return delimiter_pairs_.find(c) != delimiter_pairs_.end();
  }

  auto identifier() -> void {
    while (std::isalnum(peek()) || peek() == '_') {
      advance();
    }
    std::string txt (src_.substr(start_pos_, index_ - start_pos_));
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords_.find(txt);
    if (it != keywords_.end()) {
      type = it->second;
    }
    if (DataTypeRegistry::get_instance().is_data_type_registered(txt)) {
      type = TokenType::TYPE_IDENTIFIER;
    }
    add_token(type, txt);
  }

  auto number() -> void {
    bool is_float = false;
    std::optional<char> suffix;
    // consume integer part
    while (isdigit(peek())) {
      advance();
    }
    if (peek() == '.' && std::isdigit(peek(1))) {
      is_float = true;
      advance(); // consume the .
      // Consume the decimal part
      while (std::isdigit(peek())) {
        advance();
      }
    }

    // Handling scientific notation (1.23e-4
    if (peek() == 'e' || peek() == 'E'
      && ( std::isdigit(peek(1))
        || ((peek(1) == '+' || peek(1) == '-')
          && std::isdigit(peek(2))
          ))) {
      is_float = true;
      advance(); // Consume the 'e' or 'E'
      // Consume sign if present
      if (peek() == '+' || peek() == '-') {
        advance();
      }

      // Consume the exponent digits
      while (isdigit(peek())) {
        advance();
      }
    }

    std::string num_str (src_.substr(start_pos_, index_ - start_pos_));

    add_token(
      is_float ? TokenType::FLOAT : TokenType::INTEGER,
      num_str
    );

  }

  auto open_delimiter(const char ch) -> void {
      delimiter_stack_.push(ch);
  }

  auto close_delimiter(const char ch) -> void {
    if (delimiter_stack_.empty()) {
      throw_error("Mismatched delimiter " + std::to_string(ch));
      return;
    }
    char last_open = delimiter_stack_.top();
    if (last_open != delimiter_pairs_.at(ch)) {
      throw_error("Expected closing for " + delimiter_pairs_.at(ch));
      return;
    }
    delimiter_stack_.pop();
  }

  auto delimiter(const char ch) -> void {
    if (is_open_delimiter(ch)) {
      std::cout << "Open" << std::endl;
      open_delimiter(ch);
      return;
    }
    if (is_close_delimiter(ch)) {
      close_delimiter(ch);
      return;
    }
    if (!delimiter_stack_.empty()) {
      const char unmatched_open = delimiter_stack_.top();
      throw_error("Unmatched delimiter " + std::to_string(unmatched_open));
    }
  }

  auto skip_whitespace() -> void {
    while (true) {
      switch (char c = peek()) {
        case '\n':
          advance();
          break;
        case '/':
          if (peek(1) == '/') {
            while (peek() != '\n' && !is_end()) {
              advance();
            }
          } else if (peek(1) == '*') {
            advance(); // consume /
            advance(); // consume *

            size_t comment_start_line = line_;
            size_t comment_start_column = column_ - 2;

            while (!(peek() == '*' && peek(1) == '/') && !is_end()) {
              advance();
            }

            if (is_end()) {
              // Store current position
              size_t error_pos = start_pos_;
              size_t error_line = comment_start_line;
              size_t error_column = comment_start_column;

              throw LexerError("Unterminated block comment at ", error_line, error_column, error_pos);
            } else {
              advance(); // Consume *
              advance(); // Consume /
            }

          } else {
            return; // Not a comment, return
          }
          break;
        default:
          if (isspace(c)) {
              advance();
              break;
          }
          return; // Not whitespace
      }
    }
  }

  auto scan_token() -> void {
    skip_whitespace();
    if (is_end()) {
      return;
    }
    start_pos_ = index_;
    start_line_ = line_;
    start_column_ = column_;

    const char c = advance();

    // Handle identifiers (starting with letter or underscore)
    if (std::isalpha(c) || c == '_') {
      index_--; // Go back to re-read the first character
      column_--;
      identifier();
      return;
    }

    // Handle numbers (starting with digit or decimal point followed by digit)
    if (std::isdigit(c) || (c == '.' && std::isdigit(peek()))) {
      index_--; // Go back to re-read the first character
      column_--;
      number();
      return;
    }

    switch (c) {
      case '=':
        add_token(TokenType::EQUALS, "=");
        break;
      case ';': add_token(TokenType::SEMICOLON, ";");
        break;
      case ':':
        add_token(TokenType::COLON, ":");
        break;
      default:
        if (is_open_delimiter(c)) {
          open_delimiter(c);
          break;
        }
        if (is_close_delimiter(c)) {
          delimiter(c);
          break;
        }
        throw_error("Unexpected character '" + std::to_string(c) + "'");
        break;
    }

  }


public:

  explicit Lexer(const std::string_view src) : src_(src) {}

  auto lex() -> std::vector<Token> {
    reset();
    while (!is_end()) {
      try {
        // Mark the start of the next token
        start_pos_ = index_;
        start_line_ = line_;
        start_column_ = column_;
        scan_token();
      } catch (const LexerError& err) {
        error_occurred_ = true;
        errors_.push_back(err);
        // Try to recover by skipping to the next whitespace or known delimiter
        //recover_from_error();
      }
    }
    add_token(TokenType::END_OF_FILE, "");
    return tokens_;
  }

  // Print errors in a user-friendly format
  auto print_errors(std::ostream &os) const -> void {
    for (const auto &error: errors_) {
      os << error.what_with_location() << '\n';

      // Find the line in the source
      size_t line_start = 0;
      size_t current_line = 1;
      while (current_line < error.get_line() && line_start < src_.length()) {
        if (src_[line_start] == '\n') {
          current_line++;
        }
        line_start++;
      }

      // Find the end of the line
      size_t line_end = line_start;
      while (line_end < src_.length() && src_[line_end] != '\n') {
        line_end++;
      }

      // Extract and print the line
      std::string line = std::string(src_.substr(line_start, line_end - line_start));
      os << line << '\n';

      // Print the caret pointing to the error
      for (size_t i = 1; i < error.get_col(); i++) {
        os << ' ';
      }
      os << "^\n";
    }
  }


};

