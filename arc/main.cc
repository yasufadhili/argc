#include <stack>
#include <iostream>
#include <memory>


#include "FlexLexer.h"


#include "include/reader.hh"
#include "parser.hh"


class Lexer : public yyFlexLexer {
public:
  Lexer(std::istream* in) : yyFlexLexer(in), line_num(1), col_num(1) {}
  
  // File inclusion stack management
  std::stack<std::unique_ptr<utils::Reader>> file_stack;
  std::stack<std::istream*> stream_stack;
  std::stack<int> line_stack;
  std::stack<int> col_stack;
  std::stack<std::string> filename_stack;
  std::string current_filename;
  int line_num;
  int col_num;
    
  // Include a file and push current state onto stack
  void include_file(const std::string& filename) {
    try {
      auto reader = std::make_unique<utils::Reader>(filename);
      stream_stack.push(&yyin);
      line_stack.push(line_num);
      col_stack.push(col_num);
      filename_stack.push(current_filename);
      
      file_stack.push(std::move(reader));
      current_filename = filename;
      line_num = 1;
      col_num = 1;
      switch_streams(&file_stack.top()->stream(), nullptr);
    } catch (const std::exception& e) {
      std::cerr << "Error including file '" << filename << "': " << e.what() << std::endl;
    }
  }
    
  // Pop to previous file when EOF is reached
  bool pop_include() {
    if (!file_stack.empty() && !stream_stack.empty()) {
      switch_streams(stream_stack.top(), nullptr);
      line_num = line_stack.top();
      col_num = col_stack.top();
      current_filename = filename_stack.top();
      
      stream_stack.pop();
      line_stack.pop();
      col_stack.pop();
      filename_stack.pop();
      file_stack.pop();
      return true;
    }
    return false;
  }
  
  // Get current position info for error reporting
  std::string get_position_info() const {
    return current_filename + ":" + std::to_string(line_num) + ":" + std::to_string(col_num);
  }
};


int main(int argc, char **argv) {
  std::unique_ptr<utils::Reader> reader;
  std::istream* input = nullptr;
  try {
    if (argc > 1) {
      reader = std::make_unique<utils::Reader>(argv[1]);
    } else {
      reader = std::make_unique<utils::Reader>("");
    }
    input = &reader->stream();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  Lexer lexer(input);
  return 0;
}