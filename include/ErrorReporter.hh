#pragma once
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <string>
#include <unordered_map>
#include <mutex>
#include <source_location>
#include <vector>
#include <fstream>
#include <fmt/core.h>

/**
 * Usage Example
 *
auto exampleUsage() {
    ErrorReporter reporter(true, 100, "errors.log");
    reporter.setVerbose(true);

    // Example source location
    SourceLocation loc("main.ar", 42, 10, "var x i8 = 42.0; // Type mismatch");

    // Report a type mismatch error
    reporter.report(
        ErrorCode::TypeMismatch,
        CompileStage::TypeChecking,
        ErrorSeverity::Error,
        loc,
        "i8", "f32"
    );

    // Quick report without source location
    reporter.reportQuick(
        ErrorCode::InvalidToken,
        CompileStage::Lexing,
        ErrorSeverity::Warning,
        "Invalid character sequence"
    );

    // Check if we should continue
    if (!reporter.shouldContinue()) {
        fmt::print(stderr, "Compilation aborted due to errors\n");
    }
}
**/

namespace argc::err {
  enum class CompileStage {
    Lexing,
    Parsing,
    SymbolCollection,
    SemanticAnalysis,
    TypeChecking,
    CodeGeneration,
    Optimisation
  };

  enum class ErrorSeverity {
    Warning,
    Error,
    Fatal
  };

  struct SourceLocation {
    std::string file;
    uint32_t line;
    uint32_t column;
    std::string line_content; // Optional: store the actual line for context

    explicit SourceLocation(std::string f = "", uint32_t l = 0, uint32_t c = 0, std::string content = "")
      : file(std::move(f)), line(l), column(c), line_content(std::move(content)) {
    }
  };

  enum class ErrorCode {
    // Lexing errors
    InvalidToken,
    UnterminatedString,

    // Parsing errors
    SyntaxError,
    UnexpectedToken,

    // Symbol collection
    DuplicateSymbol,
    UndefinedSymbol,

    // Semantic analysis
    InvalidOperation,
    TypeMismatch,

    // Type checking
    IncompatibleTypes,
    MissingReturn,

    // Code generation
    InvalidInstruction,
    ResourceLimit
  };

  class ErrorTemplateDatabase {
    static inline const std::unordered_map<ErrorCode, std::string> templates{
      {ErrorCode::InvalidToken, "Invalid token encountered: {}"},
      {ErrorCode::UnterminatedString, "Unterminated string literal"},
      {ErrorCode::SyntaxError, "Syntax error: {}"},
      {ErrorCode::UnexpectedToken, "Unexpected token: {}"},
      {ErrorCode::DuplicateSymbol, "Duplicate symbol definition: {}"},
      {ErrorCode::UndefinedSymbol, "Undefined symbol: {}"},
      {ErrorCode::InvalidOperation, "Invalid operation: {}"},
      {ErrorCode::TypeMismatch, "Type mismatch: expected {}, got {}"},
      {ErrorCode::IncompatibleTypes, "Incompatible types: {} and {}"},
      {ErrorCode::MissingReturn, "Missing return statement in function {}"},
      {ErrorCode::InvalidInstruction, "Invalid instruction generated: {}"},
      {ErrorCode::ResourceLimit, "Resource limit exceeded: {}"}
    };

  public:
    static std::string getTemplate(ErrorCode code) {
      const auto it = templates.find(code);
      return it != templates.end() ? it->second : "Unknown error";
    }
  };

  class ErrorReporter {
  public:
    struct Error {
      ErrorCode code;
      ErrorSeverity severity;
      CompileStage stage;
      SourceLocation location;
      std::string message;
      std::chrono::system_clock::time_point timestamp;

      Error(ErrorCode c, ErrorSeverity s, CompileStage st, SourceLocation loc, std::string msg)
        : code(c), severity(s), stage(st), location(std::move(loc)), message(std::move(msg)),
          timestamp(std::chrono::system_clock::now()) {
      }
    };

  private:
    std::vector<Error> errors_;
    std::mutex mutex_;
    bool stop_on_error_ = false;
    size_t max_errors_ = 100;
    std::filesystem::path output_file_;
    bool verbose_ = false;

  public:
    explicit ErrorReporter(const bool stop_on_error = false, size_t max_errors = 100,
                           const std::string &output_file = "")
      : stop_on_error_(stop_on_error), max_errors_(max_errors), output_file_(output_file) {
    }

    // Configuration methods
    auto setVerbose(bool verbose) { verbose_ = verbose; }
    auto setMaxErrors(size_t max) { max_errors_ = max; }
    auto setStopOnError(bool stop) { stop_on_error_ = stop; }
    auto setOutputFile(const std::string &path) { output_file_ = path; }

    // Report error with source location
    template<typename... Args>
    auto reportError(ErrorCode code,
                     CompileStage stage,
                     ErrorSeverity severity,
                     SourceLocation loc,
                     fmt::format_string<Args...> fmt,
                     Args &&... args,
                     std::source_location src_loc = std::source_location::current()) -> void {
      std::lock_guard<std::mutex> lock(mutex_);

      if (errors_.size() >= max_errors_) {
        return;
      }

      auto message = fmt::format(fmt, std::forward<Args>(args)...);
      errors_.emplace_back(code, severity, stage, std::move(loc), std::move(message));

      printError(errors_.back(), src_loc);

      if (severity == ErrorSeverity::Fatal ||
          (stop_on_error_ && severity == ErrorSeverity::Error)) {
        throw std::runtime_error("Fatal compilation error");
      }
    }

    // Convenience method for reporting with error code template
    template<typename... Args>
    void report(ErrorCode code,
                CompileStage stage,
                ErrorSeverity severity,
                SourceLocation loc,
                Args &&... args,
                std::source_location src_loc = std::source_location::current()) {
      report_error(code, stage, severity, loc,
                   ErrorTemplateDatabase::getTemplate(code),
                   std::forward<Args>(args)...,
                   src_loc);
    }

    // Quick report without source location
    template<typename... Args>
    void reportQuick(ErrorCode code,
                     CompileStage stage,
                     ErrorSeverity severity,
                     fmt::format_string<Args...> fmt,
                     Args &&... args,
                     std::source_location src_loc = std::source_location::current()) {
      reportError(code, stage, severity, SourceLocation{}, fmt,
                   std::forward<Args>(args)..., src_loc);
    }

    // Get error statistics
    [[nodiscard]] auto errorCount() const -> size_t { return errors_.size(); }

    [[nodiscard]] auto warningCount() const -> size_t {
      return std::count_if(errors_.begin(), errors_.end(),
                           [](const Error &e) { return e.severity == ErrorSeverity::Warning; });
    }

    [[nodiscard]] auto fatalCount() const -> size_t {
      return std::count_if(errors_.begin(), errors_.end(),
                           [](const Error &e) { return e.severity == ErrorSeverity::Fatal; });
    }

    // Clear errors
    auto clear() {
      std::lock_guard<std::mutex> lock(mutex_);
      errors_.clear();
    }

    // Check if compilation should continue
    [[nodiscard]] auto shouldContinue() const -> bool {
      return errors_.size() < max_errors_ &&
             std::none_of(errors_.begin(), errors_.end(),
                          [](const Error &e) { return e.severity == ErrorSeverity::Fatal; });
    }

  private:
    auto printError(const Error &error, const std::source_location &src_loc) const -> void {
      std::string stage_str = stageToString(error.stage);
      std::string severity_str = severityToString(error.severity);

      std::string location_info;
      if (!error.location.file.empty()) {
        location_info = fmt::format("{}:{}:{}",
                                    error.location.file, error.location.line, error.location.column);
      }

      // Basic error format
      std::string output = fmt::format(
        "{}: {} in {}: {}\n",
        location_info.empty() ? "Compiler" : location_info,
        severity_str,
        stage_str,
        error.message
      );

      // Add context if available
      if (!error.location.line_content.empty()) {
        output += fmt::format("  {}\n", error.location.line_content);
        output += fmt::format("  {}^ here\n", std::string(error.location.column - 1, ' '));
      }

      // Add verbose information if enabled
      if (verbose_) {
        output += fmt::format(
          "  [ErrorCode: {}] [Time: {}] [Caller: {}:{}]\n",
          static_cast<int>(error.code),
          formatTimestamp(error.timestamp),
          src_loc.file_name(),
          src_loc.line()
        );
      }

      fmt::print(stderr, "{}", output);

      // Write to file if specified
      if (!output_file_.empty()) {
        std::ofstream out(output_file_, std::ios::app);
        if (out.is_open()) {
          out << output;
        }
      }
    }

    static auto stageToString(CompileStage stage) -> std::string {
      switch (stage) {
        case CompileStage::Lexing: return "Lexing";
        case CompileStage::Parsing: return "Parsing";
        case CompileStage::SymbolCollection: return "Symbol Collection";
        case CompileStage::SemanticAnalysis: return "Semantic Analysis";
        case CompileStage::TypeChecking: return "Type Checking";
        case CompileStage::CodeGeneration: return "Code Generation";
        case CompileStage::Optimisation: return "Optimisation";
        default: return "Unknown";
      }
    }

    static auto severityToString(ErrorSeverity severity) -> std::string {
      switch (severity) {
        case ErrorSeverity::Warning: return "warning";
        case ErrorSeverity::Error: return "error";
        case ErrorSeverity::Fatal: return "fatal error";
        default: return "unknown";
      }
    }

    static auto formatTimestamp(const std::chrono::system_clock::time_point &tp) -> std::string {
      auto t = std::chrono::system_clock::to_time_t(tp);
      std::string ts = std::ctime(&t);
      ts.resize(ts.size() - 1); // Remove trailing newline
      return ts;
    }
  };
}
