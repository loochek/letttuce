#pragma once

#include <fmt/core.h>
#include <error/compile_error.hpp>

namespace parse::error {

struct ParseError : ::error::CompileError {};

struct ParsePrimaryError : ParseError {
  explicit ParsePrimaryError(const std::string& location) {
    message = fmt::format("Could not match primary expression at location {}",
                          location);
  }
};

struct ParseTrueBlockError : ParseError {
  explicit ParseTrueBlockError(const std::string& location) {
    message =
        fmt::format("Could not parse true block at location {}", location);
  }
};

struct ParseNonLvalueError : ParseError {
  explicit ParseNonLvalueError(const std::string& location) {
    message = fmt::format("Expected lvalue at location {}", location);
  }
};

struct ParseTypeError : ParseError {
  explicit ParseTypeError(const std::string& location) {
    message = fmt::format("Could not parse the type at location {}", location);
  }
};

struct ParseTokenError : ParseError {
  ParseTokenError(const std::string& tok, const std::string& location) {
    message = fmt::format("Expected token {} at location {}", tok, location);
  }
};

struct ParseCompoundError : ParseError {
  explicit ParseCompoundError(const std::string& location) {
    message = fmt::format(
        "Some error in compound block at location {} have occured", location);
  }
};

struct ParseProgramError : ParseError {
  ParseProgramError() {
    message = "Program has some error\n";
  }
};

struct ParseDeclarationError : ParseError {
  explicit ParseDeclarationError(const std::string& location) {
    message = fmt::format("Expected declaration at location {}", location);
  }
};

}  // namespace parse::error
