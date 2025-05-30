#pragma once

#include <filesystem>
#include <vector>
#include <string>

#include "ErrorReporter.hh"

namespace argc {

class ConfigHandler {

  enum class TargetArch {
    X86_64, ARM, UNKNOWN
  };

  enum class OptimisationLevel {
    ZERO,     // No optimisation
    ONE,      // Basic
    TWO,      // Aggressive
    THREE     // Maximum
  };

  std::vector<std::string> input_files_;
  std::string output_file_;
  TargetArch target_arch_;
  OptimisationLevel optimisation_level_;
  bool emit_debug_info_;
  int8_t verbosity_level_;                // Level for diagnostics (0=none, 1 = basic, 2 = detailed)
  err::ErrorReporter& reporter_;

public:
  explicit ConfigHandler (err::ErrorReporter& reporter) :
  output_file_("a.out"),
  target_arch_(TargetArch::X86_64),
  optimisation_level_(OptimisationLevel::ONE),
  emit_debug_info_(false),
  verbosity_level_(0),
  reporter_(reporter)
  {}

  auto parseArgs (const int argc, char* argv[]) -> bool {
    for (int i =1; i < argc; ++i) {
      if (std::string arg {argv[i]}; arg == "-o" && i + 1 < argc) {
        output_file_ = argv[++i];
        if (!validateOutputFile(output_file_)) {
          reporter_.reportQuick(
            err::ErrorCode::InvalidToken,
            err::CompileStage::Lexing,
            err::ErrorSeverity::Fatal,
            "Invalid output file path: " + output_file_
            );
          return false;
        }
      } else if (arg == "-arch" && i + 1 < argc) {
        std::string arch = argv[++i];
        target_arch_ = parseTargetArch(arch);
        if (target_arch_ == TargetArch::UNKNOWN ) {
          reporter_.reportQuick(
          err::ErrorCode::InvalidToken,
          err::CompileStage::Lexing,
          err::ErrorSeverity::Fatal,
          "Unsupported target architecture: " + arch
          );
          return false;
        }
      } else if (arg == "-O0" || arg == "-O1" || arg == "-O2" || arg == "-O3") {
        optimisation_level_ = parseOptLevel(arg);
      }
      else if (arg == "-g") {
        emit_debug_info_ = true;
      }
      else if (arg == "-v") {
        verbosity_level_ = 1;
      }
      else if (arg == "-vv") {
        verbosity_level_ = 2;
      }
      else if (arg[0] != '-') {
        if (!validateInputFile(arg)) {
          reporter_.reportQuick(
          err::ErrorCode::InvalidToken,
          err::CompileStage::Lexing,
          err::ErrorSeverity::Fatal,
          "Input file does not exist: " + arg
          );
          return false;
        }
        input_files_.push_back(arg);
      }
      else {
        reporter_.reportQuick(
        err::ErrorCode::InvalidToken,
        err::CompileStage::Lexing,
        err::ErrorSeverity::Fatal,
        "Unknown option: " + arg
        );
        return false;
      }
    }
    if (input_files_.empty()) {
      reporter_.reportQuick(
        err::ErrorCode::InvalidToken,
        err::CompileStage::Lexing,
        err::ErrorSeverity::Fatal,
        "No input files provided"
      );
      return false;
    }

    return true;
  }

  // Accessors
  [[nodiscard]] const std::vector<std::string>& getInputFiles () const { return input_files_; }
  [[nodiscard]] std::string getOutputFile () const { return output_file_; }
  [[nodiscard]] TargetArch getTargetArch () const { return target_arch_; }
  [[nodiscard]] OptimisationLevel getOptimisationLevel () const { return optimisation_level_; }
  [[nodiscard]] bool shouldEmitDebugInfo () const { return emit_debug_info_; }
  [[nodiscard]] int8_t getVerbosity () const { return verbosity_level_; }

  // Convert TargetArch to string for logging or display
  [[nodiscard]] auto getTargetArchString () const -> std::string {
    switch ( target_arch_ ) {
      case TargetArch::X86_64: return "x86_64";
      case TargetArch::ARM: return "arm";
      default: return "unknown";
    }
  }

private:

  static auto validateInputFile (const std::string& filePath) -> bool {
    return std::filesystem::exists(filePath);
  }

  // Validate output file path (basic check for non-empty and writable directory)
  [[nodiscard]] static auto validateOutputFile (const std::string& filePath) -> bool {
    if (filePath.empty()) {
      return false;
    }
    std::filesystem::path path(filePath);
    auto parent = path.parent_path();
    return parent.empty() || std::filesystem::exists(parent);
  }

  // Parse target architecture string
  [[nodiscard]] static auto parseTargetArch (const std::string& arch) -> TargetArch {
    if (arch == "x86_64") return TargetArch::X86_64;
    if (arch == "arm") return TargetArch::ARM;
    return TargetArch::UNKNOWN;
  }

  // Parse optimization level string
  static auto parseOptLevel (const std::string& opt) -> OptimisationLevel {
    if (opt == "-O0") return OptimisationLevel::ZERO;
    if (opt == "-O1") return OptimisationLevel::ONE;
    if (opt == "-O2") return OptimisationLevel::TWO;
    if (opt == "-O3") return OptimisationLevel::THREE;
    return OptimisationLevel::ZERO; // Default
  }

};

}
