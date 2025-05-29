#pragma once

#include <string>

namespace argc::loc {
  struct SourceLocation {
    std::string file;
    int line;
    int column;
  };
}
