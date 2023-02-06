#pragma once

#include <lex/token_type.hpp>
#include <lex/location.hpp>

#include <fmt/core.h>

#include <string_view>
#include <filesystem>
#include <iostream>
#include <istream>
#include <vector>
#include <span>
#include <optional>

namespace lex {

//////////////////////////////////////////////////////////////////////

class Scanner {
 public:
  explicit Scanner(std::istream& source_stream) {
    // Read whole file into buffer_
    std::copy(std::istreambuf_iterator<char>(source_stream),
              std::istreambuf_iterator<char>(), std::back_inserter(buffer_));
    if (buffer_.length() == 0) {
      curr_symbol_ = EOF;
    } else {
      curr_symbol_ = buffer_[0];
    }
  }

  char CurrentSymbol() const {
    return curr_symbol_;
  }

  char NextSymbol() const {
    if (curr_loc_.abs_pos + 1 >= buffer_.length()) {
      return EOF;
    }

    return buffer_[curr_loc_.abs_pos + 1];
  }

  void MoveNext() {
    if (curr_loc_.abs_pos >= buffer_.length()) {
      curr_symbol_ = EOF;
      return;
    }

    if (curr_symbol_ == '\n') {
      curr_loc_.lineno++;
      curr_loc_.columnno = 0;
    } else {
      curr_loc_.columnno++;
    }

    curr_loc_.abs_pos++;
    if (curr_loc_.abs_pos >= buffer_.length()) {
      curr_symbol_ = EOF;
      return;
    }

    curr_symbol_ = buffer_[curr_loc_.abs_pos];
  }

  void MoveNextLine() {
    while (curr_loc_.abs_pos < buffer_.length() && buffer_[curr_loc_.abs_pos] != '\n') {
      curr_loc_.abs_pos++;
    }

    if (curr_loc_.abs_pos >= buffer_.length()) {
      curr_symbol_ = EOF;
      return;
    }

    curr_loc_.abs_pos++;
    curr_loc_.lineno++;
    curr_loc_.columnno = 0;

    if (curr_loc_.abs_pos >= buffer_.length()) {
      curr_symbol_ = EOF;
    } else {
      curr_symbol_ = buffer_[curr_loc_.abs_pos];
    }
  }

  Location GetLocation() const {
    return curr_loc_;
  }

  // [start;end)
  std::string_view GetSlice(Location start, Location end) const {
    FMT_ASSERT(start.abs_pos <= buffer_.length() &&
               end.abs_pos <= buffer_.length() &&
               start.abs_pos < end.abs_pos,
               "Could not match any token\n");

    return std::string_view(&buffer_[start.abs_pos], end.abs_pos - start.abs_pos);
  }

 private:
  char curr_symbol_ = EOF;

  std::string buffer_;
  Location curr_loc_;
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
