#pragma once

#include <fmt/core.h>
#include <error/compile_error.hpp>

namespace error {
class ErrorHandler {
 public:
  static ErrorHandler& GetInstance() {
    static ErrorHandler handler;
    return handler;
  }

  void ReportCompileError(const error::CompileError& error) {
    fmt::print("Error: {}\n", error.what());
  }

 private:
  ErrorHandler() = default;
};
} // namespace error