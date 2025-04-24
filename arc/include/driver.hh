#pragma once
#include "config.hh"
#include "../src/parser.hh"
#include "ast.hh"

namespace driver {


class OptionParser {
  int argc_;
  char** argv_;
public:
  OptionParser(int argc, char* argv[]);
  ~OptionParser();
  auto parse(config::Config& config) const -> bool;
};

auto display_help(const std::string& prog_name) -> void;
auto display_version() -> void;

auto validate_input_files(const config::Config& config) -> bool;
auto cleanup_temp_files(const config::Config& config) -> void;


}

