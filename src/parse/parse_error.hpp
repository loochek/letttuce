#pragma once

#include <fmt/core.h>

#include <string>

namespace parse::errors {

struct ParseError : std::exception {
  std::string message;

  const char* what() const noexcept override {
    return message.c_str();
  }
};

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
        "Some errors in compound block at location {} have occured", location);
  }
};

struct ParseProgramError : ParseError {
  ParseProgramError() {
    message = "Program has some errors\n";
  }
};

struct ParseDeclarationError : ParseError {
  explicit ParseDeclarationError(const std::string& location) {
    message = fmt::format("Expected declaration at location {}", location);
  }
};

}  // namespace parse::errors
