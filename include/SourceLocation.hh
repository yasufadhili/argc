#pragma once

#include <cstdint>
#include <string>

namespace argc::loc {
  struct SourceLocation {
    std::string file;
    uint32_t line;
    uint32_t column;
  };
}
