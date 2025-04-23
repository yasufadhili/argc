
#include <set>
#include <filesystem>
#include <iostream>

#include "../include/driver.hh"

namespace fs = std::filesystem;
using namespace driver;

auto driver::display_help(const std::string &prog_name) -> void {
  std::cout << "Usage: " << prog_name << " [options] file..." << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -c                    Compile and assemble, but do not link" << std::endl;
  std::cout << "  -E                    Preprocess only; do not compile, assemble or link" << std::endl;
  std::cout << "  -S                    Compile only; do not assemble or link" << std::endl;
  std::cout << "  -fsyntax-only         Check syntax only; do not compile, assemble or link" << std::endl;
  std::cout << "  -o <file>             Place the output into <file>" << std::endl;
  std::cout << "  -O<level>             Set optimisation level (0-3)" << std::endl;
  std::cout << "  -g                    Generate debugging information" << std::endl;
  std::cout << "  -w                    Disable all warnings" << std::endl;
  std::cout << "  -Werror               Treat warnings as errors" << std::endl;
  std::cout << "  -std=<standard>       Specify language standard (e.g., 1)" << std::endl;
  std::cout << "  -I<dir>               Add directory to include search path" << std::endl;
  //std::cout << "  -D<macro>[=<val>]     Define a preprocessor macro" << std::endl;
  std::cout << "  -L<dir>               Add directory to library search path" << std::endl;
  std::cout << "  -l<lib>               Link with library <lib>" << std::endl;
  std::cout << "  -static               Link statically rather than dynamically" << std::endl;
  //std::cout << "  --arch=<arch>         Specify target architecture" << std::endl;
  std::cout << "  --keep-temp           Keep temporary files" << std::endl;
  std::cout << "  -v, --verbose         Enable verbose output" << std::endl;
  std::cout << "  --help                Display this information" << std::endl;
  std::cout << "  --version             Display compiler version information" << std::endl;
}

auto driver::display_version() -> void {
  std::cout << "Argon Compiler (argc) v0.0.1" << std::endl;
  std::cout << "No Copyright (C) 2025 Yasu Fadhili" << std::endl;
  std::cout << "This is free software; see the source for copying conditions." << std::endl;
}

auto driver::validate_input_files(const config::Config &config) -> bool {
  std::set<std::string> valid_extensions { ".ar", ".arg", ".argon" };
  for (const auto& file : config.input_files) {
    if (!fs::exists(file)) {
      std::cerr << "Error: Input file '" << file << "' does not exist" << std::endl;
      return false;
    }
    std::string extension { fs::path(file).extension().string() };
    if (valid_extensions.find(extension) == valid_extensions.end()) {
      std::cerr << "Warning: File '" << file << "' has unrecognised extension '" << extension << "'" << std::endl;
    }
  }
  return true;
}

auto driver::cleanup_temp_files(const config::Config& config) -> void {
  if (!config.keep_temp_files) {
    for (const auto& file : config.temp_files) {
      //std::remove(file.c_str());
      if (fs::exists(file)) {
        try {
          fs::remove(file);
          if (config.verbose) {
            std::cout <<  "Removed temporary file: " << file << std::endl;
          }
        } catch (const std::exception &e) {
          std::cerr << "Warning: Failed to remove temporary file '" << file << "': " << e.what() << std::endl;
        }
      }
    }
  }
}

