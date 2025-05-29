#pragma once

#include <string>

namespace argc::loc {
  struct SourceLocation {
    std::string file;
    int line;
    int column;

  public:
    SourceLocation(int line, int column, const std::string& file = "")
      : file(file), line(line), column(column) {}
  };
}
