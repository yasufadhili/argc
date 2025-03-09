#pragma once


struct SourceLocation {
  size_t line {1};
  size_t column {1};
  size_t index {0};

  SourceLocation(const size_t line, const size_t column, const size_t index)
    : line(line), column(column), index(index) {
  }
};

