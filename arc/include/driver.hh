#pragma once
#include "config.hh"

namespace driver {


class OptionParser {
  int argc_;
  char** argv_;
public:
  OptionParser(int argc, char* argv[]);
  ~OptionParser();
  auto parse(conf::Config& config) -> bool;
};

auto display_help(const std::string& prog_name) -> void;
auto display_version() -> void;

auto validate_input_files(const conf::Config& config) -> bool;
auto cleanup_temp_files(conf::Config& config) -> void;



class Compiler {
  conf::Config config_;
  auto log_config() -> void;
  auto get_build_mode_name() -> std::string;
  auto preprocess_files() -> bool;
  auto check_syntax() -> bool;
  auto generate_asm() -> bool;
  auto compile_to_obj_files() -> bool;
  auto compile_and_link() -> bool;
public:
  explicit Compiler(const conf::Config& config);
  ~Compiler() = default;
  auto compile() -> bool;
};



}

