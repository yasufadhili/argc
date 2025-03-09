#pragma once

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

using String = std::string;
using StringView = std::string_view;
using RuntimeError = std::runtime_error;
using StringStream = std::stringstream;

/**
 * Reads the entire contents of a file into a string
 * @param path Path to the file
 * @return String containing file contents
 * @throws RuntimeError if the file couldn't be opened
 */
inline auto read_file_to_string(const String &path) -> String {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    throw RuntimeError("Failed to open file: " + path);
  }
  StringStream buf;
  buf << file.rdbuf();
  return buf.str();
}