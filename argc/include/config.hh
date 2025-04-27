#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace config {


struct Config {
  std::ifstream input_file;
  std::vector<std::string> input_files;
  std::string output_file { "a.out" };
  bool output_specified { false } ;
  std::vector<std::string> output_files;


  enum class BuildMode {
    COMPILE_ONLY,       // -c
    COMPILE_AND_EXEC,   // Not yet
    COMPILE_AND_LINK,   // default
    PREPROCESS,         // -E
    ASSEMBLE_ONLY,      // -S
    CHECK_SYNTAX_ONLY   // -fsyntax-only
  } build_mode = BuildMode::COMPILE_AND_LINK;

  // Preprocessor Options
  std::vector<std::string> include_paths;
  std::unordered_map<std::string, std::string> defined_macros;

  int8_t optimisation_level { -1 };
  bool warnings { true } ;
  bool warnings_as_errors { false } ;
  bool debug { false } ;
  std::string standard_version { "1" };
  std::string target_architecture { "x86_64" };

  // Linker options
  std::vector<std::string> library_paths;
  std::vector<std::string> libraries;
  bool static_linking { false } ;

  // Misc Options
  bool verbose { false } ;
  bool show_help { false } ;
  bool show_version { false } ;

  std::vector<std::string> temp_files;
  bool keep_temp_files { false } ;

};



}

