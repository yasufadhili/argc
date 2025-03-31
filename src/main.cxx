
#include <string>

#include "lexer/lexer.hxx"
#include "parser/parser.hxx"
#include "utils/reader.hxx"
#include "utils/writer.hxx"


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
  try {
    if (!utils::reader::file_exists(filename)) {
      return Result::error(ErrorCode::FILE_NOT_FOUND, "File not found: " + filename);
    }

    utils::reader::FileReader reader(filename);
    content = reader.read_all();
    return Result::ok();
  }
  catch (const std::exception& e) {
    return Result::error(ErrorCode::UNKNOWN_ERROR, std::string("Error reading file: ") + e.what());
  }
}

auto main(const int argc, const char* argv[]) -> int {
  std::string filename;
  std::string outfile = "output";
  
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input_file.ar> [output_file]\n", argv[0]);
    return 1;
  }
  
  filename = argv[1];
  
  if (argc >= 3) {
    outfile = argv[2];
  }
  
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

  //for (const auto& t : lxr.tokenise()) {
  //  printf(t.to_string().c_str());
  //  printf("\n");
  //}
  parser::Parser psr("", lxr.tokenise());
  std::shared_ptr<ast::prog::Program> program_ast = psr.parse();

  ast::Visitor visitor;
  program_ast->accept(visitor);

  try {
    utils::writer::FileWriter writer(outfile);
    if (!writer.write(visitor.get_output())) {
      fprintf(stderr, "Failed to write to output file: %s\n", outfile.c_str());
      return 1;
    }
  }
  catch (const std::exception& e) {
    fprintf(stderr, "Error writing to output file: %s\n", e.what());
    return 1;
  }

  return 0;
}