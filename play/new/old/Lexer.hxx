#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <optional>

#include "TokenType.hxx"
#include "SourceLocation.hxx"
#include "Token.hxx"


class LexerError final : public std::runtime_error {
public:
  LexerError(const std::string &message, size_t line, size_t column, size_t position)
    : std::runtime_error(message), line_(line), column_(column), position_(position) {
  }

  [[nodiscard]] auto get_line() const -> size_t { return line_; }
  [[nodiscard]] auto get_column() const -> size_t { return column_; }
  [[nodiscard]] auto get_position() const -> size_t { return position_; }

  [[nodiscard]] auto what_with_location() const -> std::string {
    std::ostringstream oss;
    oss << what() << " at line " << line_ << ", column " << column_
        << " (position " << position_ << ")";
    return oss.str();
  }

private:
  size_t line_;
  size_t column_;
  size_t position_;
};

class Lexer {
public:
  explicit Lexer(std::string_view source) : src_(source) {
  }

  auto lex() -> std::vector<Token> {
    reset();
    while (!is_end()) {
      try {
        // Mark the start of the next token
        start_pos_ = index_;
        start_line_ = line_;
        start_column_ = column_;
        scan_token();
      } catch (const LexerError &e) {
        error_occurred_ = true;
        errors_.push_back(e);
        // Try to recover by skipping to the next whitespace or known delimiter
        recover_from_error();
      }
    }

    add_token(TokenType::END_OF_FILE, "");
    return tokens_;
  }

  [[nodiscard]] auto has_errors() const -> bool { return error_occurred_; }

  [[nodiscard]] auto get_errors() const -> const std::vector<LexerError> & {
    return errors_;
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
      for (size_t i = 1; i < error.get_column(); i++) {
        os << ' ';
      }
      os << "^\n";
    }
  }

private:
  std::string_view src_{};
  size_t line_{1};
  size_t column_{1};
  size_t index_{0};
  size_t start_pos_{0};
  size_t start_line_{1};
  size_t start_column_{1};

  std::vector<Token> tokens_{};
  bool error_occurred_{false};
  std::vector<LexerError> errors_{};

  // Maps of keywords and single character tokens for quick lookup
  const std::unordered_map<std::string, TokenType> keywords_{
    {"return", TokenType::RETURN},
  };

  void reset() {
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

  auto recover_from_error() -> void {
    // Skip until we find whitespace or a known delimiter
    while (!is_end()) {
      char c = peek();
      if (std::isspace(c) || c == ';' || c == ',' || c == ')' || c == '}' || c == ']') {
        break;
      }
      advance();
    }

    // If we found a delimiter, consume it to help synchronize
    if (!is_end() && !std::isspace(peek())) {
      advance();
    }
  }

  auto identifier() -> void {
    while (std::isalnum(peek()) || peek() == '_') {
      advance();
    }

    const std::string text(src_.substr(start_pos_, index_ - start_pos_));

    // Check if identifier is a keyword
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords_.find(text);
    if (it != keywords_.end()) {
      type = it->second;
    }

    add_token(type, text);
  }

  auto number() -> void {
    bool is_float = false;
    std::optional<char> suffix;

    // Consume integer part
    while (std::isdigit(peek())) {
      advance();
    }

    // Look for decimal point followed by at least one digit
    if (peek() == '.' && std::isdigit(peek(1))) {
      is_float = true;
      advance(); // Consume the '.'

      // Consume decimal part
      while (std::isdigit(peek())) {
        advance();
      }
    }

    // Handle scientific notation (e.g., 1.23e-4)
    if ((peek() == 'e' || peek() == 'E') &&
        (std::isdigit(peek(1)) ||
         ((peek(1) == '+' || peek(1) == '-') && std::isdigit(peek(2))))) {
      is_float = true;
      advance(); // Consume 'e' or 'E'

      // Consume sign if present
      if (peek() == '+' || peek() == '-') {
        advance();
      }

      // Consume exponent digits
      while (std::isdigit(peek())) {
        advance();
      }
    }

    // Check for suffixes (f, u, l, etc.)
    if (std::isalpha(peek())) {
      suffix = peek();
      advance();

      // Handle double-letter suffixes like "ll" for long long
      if ((suffix == 'l' || suffix == 'L') && (peek() == 'l' || peek() == 'L')) {
        advance();
      }
    }

    std::string number_str(src_.substr(start_pos_, index_ - start_pos_));
    add_token(is_float ? TokenType::FLOAT : TokenType::INTEGER, number_str);
  }

  auto character() -> void {
    // Skip the opening quote
    advance();

    std::string char_repr;
    char value = '\0';
    bool escaped = false;

    // Handle empty character literal
    if (peek() == '\'') {
      throw_error("Empty character literal");
    }

    // Handle escaped characters
    if (peek() == '\\') {
      escaped = true;
      char_repr += '\\';
      advance(); // Consume backslash

      switch (peek()) {
        case 'n': value = '\n';
          char_repr += 'n';
          break;
        case 't': value = '\t';
          char_repr += 't';
          break;
        case 'r': value = '\r';
          char_repr += 'r';
          break;
        case '\\': value = '\\';
          char_repr += '\\';
          break;
        case '\'': value = '\'';
          char_repr += '\'';
          break;
        case '0': value = '\0';
          char_repr += '0';
          break;
        case 'x': {
          // Handle hexadecimal escape sequence \xHH
          char_repr += 'x';
          advance(); // Consume 'x'
          std::string hex_digits;
          // Read exactly 2 hex digits
          for (int i = 0; i < 2 && std::isxdigit(peek()); ++i) {
            hex_digits += peek();
            char_repr += peek();
            advance();
          }
          if (hex_digits.length() < 2) {
            throw_error("Incomplete hexadecimal escape sequence");
          }
          // Convert hex to character value
          value = static_cast<char>(std::stoi(hex_digits, nullptr, 16));
          index_--; // Adjust for the upcoming advance() after the switch
          column_--;
          break;
        }
        default:
          throw_error("Invalid escape sequence");
      }
    } else {
      value = peek();
      char_repr += value;
    }

    advance(); // Consume the character or escaped character

    // Expect closing quote
    if (peek() != '\'') {
      throw_error("Unterminated character literal");
    } else {
      advance(); // Consume closing quote
    }

    // Store both the character representation and its actual value
    add_token_with_char_info(TokenType::CHAR, char_repr, value);
  }

  auto string() -> void {
    // Skip the opening quote
    advance();

    std::string value;
    std::string raw_content; // Store the raw string content including escape sequences

    while (peek() != '"' && !is_end()) {
      if (peek() == '\\') {
        raw_content += '\\';
        advance(); // Consume backslash

        switch (peek()) {
          case 'n': value += '\n';
            raw_content += 'n';
            break;
          case 't': value += '\t';
            raw_content += 't';
            break;
          case 'r': value += '\r';
            raw_content += 'r';
            break;
          case '\\': value += '\\';
            raw_content += '\\';
            break;
          case '"': value += '"';
            raw_content += '"';
            break;
          case '0': value += '\0';
            raw_content += '0';
            break;
          case 'x': {
            // Handle hexadecimal escape sequence \xHH
            raw_content += 'x';
            advance(); // Consume 'x'
            std::string hex_digits;
            // Read exactly 2 hex digits
            for (int i = 0; i < 2 && std::isxdigit(peek()); ++i) {
              hex_digits += peek();
              raw_content += peek();
              advance();
            }
            if (hex_digits.length() < 2) {
              throw_error("Incomplete hexadecimal escape sequence in string");
            }
            // Convert hex to character value
            value += static_cast<char>(std::stoi(hex_digits, nullptr, 16));
            index_--; // Adjust for the upcoming advance() after the switch
            column_--;
            break;
          }
          default:
            throw_error("Invalid escape sequence in string");
        }
      } else {
        char c = peek();
        if (c == '\n') {
          throw_error("Newline in string literal");
        }
        value += c;
        raw_content += c;
      }

      advance();
    }

    if (is_end()) {
      throw_error("Unterminated string");
    }

    // Skip the closing quote
    advance();

    // Store both the processed value and the raw content
    add_token_with_string_info(TokenType::STRING, raw_content, value);
  }

  auto skip_whitespace() -> void {
    while (true) {
      switch (char c = peek()) {
        case ' ':
        case '\t':
        case '\r':
          advance();
          break;
        case '\n':
          advance();
          break;
        case '/':
          if (peek(1) == '/') {
            // Line comment: skip to end of line
            while (peek() != '\n' && !is_end()) {
              advance();
            }
          } else if (peek(1) == '*') {
            // Block comment: skip until */
            advance(); // Consume /
            advance(); // Consume *

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

              // Throw error with the comment start position
              throw LexerError("Unterminated block comment",
                               error_line, error_column, error_pos);
            } else {
              advance(); // Consume *
              advance(); // Consume /
            }
          } else {
            return; // Not a comment, return
          }
          break;
        default:
          return; // Not whitespace, return
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

    // Handle different token types based on the character
    switch (c) {
      // Potentially two-character tokens
      case '!':
        add_token(match('=') ? TokenType::NOT_EQUAL : TokenType::BANG,
                  match('=') ? "!=" : "!");
        break;
      case '=':
        add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL,
                  match('=') ? "==" : "=");
        break;
      case '<':
        if (match('=')) {
          add_token(TokenType::LESS_EQUAL, "<=");
        } else if (match('<')) {
          add_token(TokenType::LEFT_SHIFT, "<<");
        } else {
          add_token(TokenType::LESS, "<");
        }
        break;
      case '>':
        if (match('=')) {
          add_token(TokenType::GREATER_EQUAL, ">=");
        } else if (match('>')) {
          add_token(TokenType::RIGHT_SHIFT, ">>");
        } else {
          add_token(TokenType::GREATER, ">");
        }
        break;
      case '+':
        if (match('+')) {
          add_token(TokenType::PLUS_PLUS, "++");
        } else if (match('=')) {
          add_token(TokenType::PLUS_EQUAL, "+=");
        } else {
          add_token(TokenType::PLUS, "+");
        }
        break;
      case '-':
        if (match('-')) {
          add_token(TokenType::MINUS_MINUS, "--");
        } else if (match('=')) {
          add_token(TokenType::MINUS_EQUAL, "-=");
        } else if (match('>')) {
          add_token(TokenType::ARROW, "->");
        } else {
          add_token(TokenType::MINUS, "-");
        }
        break;
      case '*':
        add_token(match('=') ? TokenType::STAR_EQUAL : TokenType::STAR,
                  match('=') ? "*=" : "*");
        break;
      case '/':
        add_token(match('=') ? TokenType::SLASH_EQUAL : TokenType::SLASH,
                  match('=') ? "/=" : "/");
        break;
      case '%':
        add_token(match('=') ? TokenType::PERCENT_EQUAL : TokenType::PERCENT,
                  match('=') ? "%=" : "%");
        break;
      case '&':
        if (match('&')) {
          add_token(TokenType::AND_AND, "&&");
        } else if (match('=')) {
          add_token(TokenType::AND_EQUAL, "&=");
        } else {
          add_token(TokenType::AND, "&");
        }
        break;
      case '|':
        if (match('|')) {
          add_token(TokenType::OR_OR, "||");
        } else if (match('=')) {
          add_token(TokenType::OR_EQUAL, "|=");
        } else {
          add_token(TokenType::OR, "|");
        }
        break;
      case '^':
        add_token(match('=') ? TokenType::XOR_EQUAL : TokenType::XOR,
                  match('=') ? "^=" : "^");
        break;
      case '~':
        add_token(TokenType::TILDE, "~");
        break;

      // String literals
      case '"':
        index_--; // Go back to re-read the quote
        column_--;
        string();
        break;

      // Character literals
      case '\'':
        index_--; // Go back to re-read the quote
        column_--;
        character();
        break;

      // Simple single-character tokens
      case '(': add_token(TokenType::LEFT_PAREN, "(");
        break;
      case ')': add_token(TokenType::RIGHT_PAREN, ")");
        break;
      case '{': add_token(TokenType::LEFT_BRACE, "{");
        break;
      case '}': add_token(TokenType::RIGHT_BRACE, "}");
        break;
      case '[': add_token(TokenType::LEFT_BRACKET, "[");
        break;
      case ']': add_token(TokenType::RIGHT_BRACKET, "]");
        break;
      case ';': add_token(TokenType::SEMICOLON, ";");
        break;
      case ':':
        add_token(match(':') ? TokenType::COLON_COLON : TokenType::COLON,
                  match(':') ? "::" : ":");
        break;
      case ',': add_token(TokenType::COMMA, ",");
        break;
      case '.': add_token(TokenType::DOT, ".");
        break;
      case '?': add_token(TokenType::QUESTION, "?");
        break;

      default:
        throw_error("Unexpected character");
        break;
    }
  }

  auto throw_error(const std::string &message) const -> void {
    throw LexerError(message, start_line_, start_column_, start_pos_);
  }

  auto add_token(TokenType type, const std::string &value) -> void {
    tokens_.emplace_back(type, value, SourceLocation(start_line_, start_column_, start_pos_));
  }

  auto add_token_with_char_info(TokenType type, const std::string &repr, char value) -> void {
    // Assume Token constructor has been updated to accept character value
    tokens_.emplace_back(type, repr, SourceLocation(start_line_, start_column_, start_pos_), value);
  }

  auto add_token_with_string_info(TokenType type, const std::string &repr, const std::string &value) -> void {
    // Assume Token constructor has been updated to accept string value
    tokens_.emplace_back(type, repr, SourceLocation(start_line_, start_column_, start_pos_), value);
  }
};
