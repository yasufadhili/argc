
#include <fstream>
#include <filesystem>
#include <string>

#include "lexer/lexer.hxx"
#include "parser/parser.hxx"


enum class ErrorCode {
  SUCCESS,
  FILE_NOT_FOUND,
  ACCESS_DENIED,
  IO_ERROR,
  INVALID_ARGUMENT,
  UNKNOWN_ERROR
};

struct Result {
  ErrorCode code;
  std::string message;

  [[nodiscard]] bool success() const { return code == ErrorCode::SUCCESS; }

  static Result ok() {
    return {ErrorCode::SUCCESS, ""};
  }

  static Result error(const ErrorCode code, const std::string& msg) {
    return {code, msg};
  }
};

auto get_file_extension(const std::string& filename) -> std::string {
  if (const size_t dotPos = filename.find_last_of('.'); dotPos != std::string::npos) {
    return filename.substr(dotPos);
  }
  return "";
}

auto read_file_to_string(const std::string& filename, std::string& content) -> Result {
  if (!std::filesystem::exists(filename)) {
    return Result::error(ErrorCode::FILE_NOT_FOUND, "File not found: " + filename);
  }

  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    return Result::error(ErrorCode::ACCESS_DENIED, "Cannot access file: " + filename);
  }

  try {
    std::stringstream buffer;
    buffer << file.rdbuf();
    content = buffer.str();

    if (file.bad()) {
      return Result::error(ErrorCode::IO_ERROR, "I/O error while reading file: " + filename);
    }

    return Result::ok();
  }
  catch (const std::exception& e) {
    return Result::error(ErrorCode::UNKNOWN_ERROR, std::string("Error reading file: ") + e.what());
  }
}

auto main(const int argc, const char* argv[]) -> int {

  const std::string filename {"../../examples/1_ret.ar"};
  std::string outfile { "out" };
  std::string content;

  if (get_file_extension(filename) != ".ar") {
    fprintf(stderr, "Unrecognised file extension. Expected .ar file \n");
    return 1;
  }

  if (const auto result = read_file_to_string(filename, content); !result.success()) {
    fprintf(stderr, "Error: %s\n", result.message.c_str());
    return 1;
  }

  lexer::Lexer lxr ("", content);
  //std::vector<token::Token> tokens { lxr.tokenise() };

  parser::Parser psr("", lxr.tokenise());
  std::shared_ptr<ast::prog::Program> program_ast = psr.parse();

  ast::Visitor visitor;
  program_ast->accept(visitor);

  printf(visitor.get_output().c_str());

  return 0;
}