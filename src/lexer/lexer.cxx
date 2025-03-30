
#include <unordered_map>

#include "lexer.hxx"

#include "../token/token.hxx"

namespace lexer {

  using namespace token;

  inline std::unordered_map<char, TokenType> delimiters_map {
    {'{', TokenType::LEFT_BRACE},
    {'}', TokenType::RIGHT_BRACE},
    {'(', TokenType::LEFT_PAREN},
    {')', TokenType::RIGHT_PAREN},
    {'[', TokenType::LEFT_BRACKET},
    {']', TokenType::RIGHT_BRACKET},
  };

  inline std::unordered_map<char, TokenType> single_char_map {
    {':', TokenType::COLON},
    {';', TokenType::SEMICOLON},
    {',', TokenType::COMMA},
    {'+', TokenType::PLUS},
    {'-', TokenType::MINUS},
    {'>', TokenType::GT},
    {'<', TokenType::LT},
    {'?', TokenType::QUESTION},
  };

  inline std::unordered_map<std::string, TokenType> keywords_map{
    {"def", TokenType::DEF},
    {"ret", TokenType::RET},
    {"module", TokenType::MODULE},
    {"import", TokenType::IMPORT},
    {"var", TokenType::VAR}
  };


  auto Lexer::create_error(const std::string &msg) const -> void {
    fprintf(stderr, "Error: %s at line %d:%d in %s", msg.c_str(), static_cast<int>(line_), static_cast<int>(col_), file_.c_str());
    //printf("\n");
    exit(EXIT_FAILURE);
  }

  auto Lexer::create_error(const std::string &msg, const std::string &lexeme) const -> void {
    const std::string err = msg + " '" + lexeme + "'";
    create_error(err);
  }

  auto Lexer::create_error(const std::string &msg, const size_t line, const size_t col) const -> void {
    fprintf(stderr, "Error: %s at line %d:%d in %s", msg.c_str(), static_cast<int>(line), static_cast<int>(col), file_.c_str());
    //printf("\n");
    exit(EXIT_FAILURE);
  }

  auto Lexer::is_eof() const -> bool {
    return index_ >= src_.length() || src_[index_] == '\0';
  }

  auto Lexer::is_eol() const -> bool {
    return src_[index_] == '\n';
  }

  auto Lexer::peek(const size_t offset = 0) const -> char {
    if (index_ + offset >= src_.length()) return '\0';
    return src_[index_ + offset];
  }

  auto Lexer::advance() -> char {
    if (is_eof()) return '\0';
    const char ch = peek();
    index_++;
    col_++;
    if (ch == '\n') {
      line_++;
      col_ = 1;
    }
    return ch;
  }

  auto Lexer::add_token(TokenType type, std::string &lexeme) -> void {
    tokens_.emplace_back(type, lexeme, start_line_, start_col_, start_index_);
  }

  auto Lexer::add_token(TokenType type, const char *text) -> void {
    tokens_.emplace_back(type, text, start_line_, start_col_, start_index_);
  }

  auto Lexer::skip_whitespace() -> void {
    while (true) {
      switch (const char c = peek()) {
        case '\n':
          //add_token(TokenType::END_OF_LINE, "\\n");
          advance();
        break;
        case '/':
          if (peek(1) == '/') {
            while (peek() != '\n' && !is_eof()) {
              advance();
            }
          } else if (peek(1) == '*') {
            advance(); // consume /
            advance(); // consume *

            const size_t comment_start_line = line_;
            const size_t comment_start_column = col_ - 2;

            while (!(peek() == '*' && peek(1) == '/') && !is_eof()) {
              advance();
            }

            if (is_eof()) {
              // Store curr position
              //size_t error_pos = start_index_;
              const size_t error_line = comment_start_line;
              const size_t error_column = comment_start_column;

              create_error("Unterminated block comment at ", error_line, error_column);
            }

            advance(); // Consume *
            advance(); // Consume /
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

  auto Lexer::is_delimiter(const char ch) -> bool {
    return delimiters_map.contains(ch);
  }

  auto Lexer::is_open_delimiter(const char ch) -> bool {
    return ch == '{' || ch == '[' || ch == '(';
  }

  auto Lexer::is_close_delimiter(const char ch) -> bool {
    return ch == '}' || ch == ']' || ch == ')';
  }

  auto Lexer::delimiter_token() -> void {
    if (const char curr = peek(); is_open_delimiter(curr)) {
      delimiter_stack_.push(curr);
      std::string value(1, curr);
      advance(); // Consume the delimiter
      add_token(delimiters_map[curr], value);
    } else if (is_close_delimiter(curr)) {
      if (delimiter_stack_.empty()) {
        create_error("Unmatched closing delimiter", std::string(1, curr));
        return;
      }

      char expected_opener = 0;
      if (curr == ')') expected_opener = '(';
      else if (curr == '}') expected_opener = '{';
      else if (curr == ']') expected_opener = '[';

      if (const char actual_opener = delimiter_stack_.top(); actual_opener != expected_opener) {
        const std::string error_msg = "Mismatched delimiter. Found '" + std::string(1, curr) + "' but expected closing delimiter for '" + std::string(1, actual_opener) + "'";
        create_error(error_msg);
        return;
      }

      delimiter_stack_.pop();
      std::string value(1, curr);
      advance(); // Consume the delimiter
      add_token(delimiters_map[curr], value);
    }
  }

  auto Lexer::is_single_char(const char ch) -> bool {
    return single_char_map.contains(ch);
  }

  auto Lexer::single_char_token() -> void {
    const char curr = peek();
    std::string value = std::string(1, curr);
    advance(); // Consume the character
    add_token(single_char_map[curr], value);
  }

  auto Lexer::identifier_token() -> void {
    while (std::isalnum(peek()) || peek() == '_') {
      advance();
    }
    std::string txt(src_.substr(start_index_, index_ - start_index_));
    TokenType type = TokenType::IDENTIFIER;
    if (const auto it = keywords_map.find(txt); it != keywords_map.end()) {
      type = it->second;
    }
    //if (registry::DataTypeRegistry::get_instance().is_data_type_registered(txt)) {
    //  type = TokenType::TYPE_IDENTIFIER;
    //}
    add_token(type, txt);
  }

  auto Lexer::number_token() -> void {
    bool is_float = false;
    //std::optional<char> suffix;

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

    // Handling scientific notation (1.23e-4)
    if ((peek() == 'e' || peek() == 'E')
        && (std::isdigit(peek(1))
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

    std::string num_str(src_.substr(start_index_, index_ - start_index_));

    add_token(
      is_float ? TokenType::FLOAT : TokenType::INTEGER,
      num_str
    );
  }

  auto Lexer::scan_token() -> void {
    skip_whitespace();

    if (is_eof()) return;

    const char ch = peek();

    start_index_ = index_;
    start_line_ = line_;
    start_col_ = col_;

    //if (ch == '"') {
    //  string();
    //  return;
    //}

    //if (is_operator(ch)) {
    //  operator_token();
    //  return;
    //}

    if (ch == '-' && peek(1) == '>') {
      advance();
      advance();
      add_token(TokenType::RIGHT_ARROW, "->");
      return;
    }

    if (is_single_char(ch)) {
      single_char_token();
      return;
    }

    if (std::isdigit(ch) || (ch == '.' && std::isdigit(peek(1)))) {
      number_token();
      return;
    }

    if (std::isalpha(ch) || ch == '_') {
      identifier_token();
      return;
    }

    if (is_delimiter(ch)) {
      delimiter_token();
      return;
    }

    if (ch == '\0') return; // end of file do nothing

    // If we get here, it's an unrecognised character
    const std::string error_char(1, ch);
    advance(); // Consume the character so we don't get stuck
    create_error("Unexpected character", error_char);
  }

  auto Lexer::tokenise() -> std::vector<Token> {
    while (!is_eof()) {
      scan_token();
    }

    if (!delimiter_stack_.empty()) {
      const char unclosed = delimiter_stack_.top();
      const std::string delimiter_str(1, unclosed);

      // Find the position of the unclosed delimiter in the source
      const size_t unclosed_pos = src_.rfind(unclosed);
      size_t unclosed_line = 1;
      size_t unclosed_col = 1;

      // Approximate line and column for error reporting
      for (size_t i = 0; i < unclosed_pos; i++) {
        if (src_[i] == '\n') {
          unclosed_line++;
          unclosed_col = 1;
        } else {
          unclosed_col++;
        }
      }

      fprintf(stderr, "Error: Unclosed delimiter '%s' opened at %d:%d ", delimiter_str.c_str(),
              static_cast<int>(unclosed_line), static_cast<int>(unclosed_col));
      exit(1);
    }

    tokens_.emplace_back(TokenType::END_OF_FILE, "", line_, col_, index_);

    return tokens_;
  }

}
