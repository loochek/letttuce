#pragma once

#include <fmt/core.h>

#include <cstddef>
#include <string>

namespace lex {
struct Location {
  size_t lineno = 0;
  size_t columnno = 0;
  size_t abs_pos = 0;

  Location() = default;
  Location(size_t lineno, size_t columnno, size_t abs_pos)
      : lineno(lineno), columnno(columnno), abs_pos(abs_pos) {
  }

  std::string Format() const {
    return fmt::format("line {}, column {}",  lineno + 1, columnno + 1);
  }
};
}  // namespace lex
