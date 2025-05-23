#pragma once

#include <filesystem>
#include <vector>
#include <string>

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

public:
  ConfigHandler () :
  output_file_("a.out"),
  target_arch_(TargetArch::X86_64),
  optimisation_level_(OptimisationLevel::ONE),
  emit_debug_info_(false),
  verbosity_level_(0)
  {}

  auto parseArgs (const int argc, char* argv[]) -> bool {
    for (int i =1; i < argc; ++i) {
      std::string arg {argv[i]};
      if (arg == "-o" && i + i < argc) {
        output_file_ = argv[++i];
        if (!validateOutputFile(output_file_)) {
          // TODO Report Error
          return false;
        }
      }
    }
    return true;
  }

private:

  bool validateInputFile(const std::string& filePath) const {
    return std::filesystem::exists(filePath);
  }

  // Validate output file path (basic check for non-empty and writable directory)
  [[nodiscard]] static auto validateOutputFile(const std::string& filePath) -> bool {
    if (filePath.empty()) {
      return false;
    }
    std::filesystem::path path(filePath);
    auto parent = path.parent_path();
    return parent.empty() || std::filesystem::exists(parent);
  }

};

}
