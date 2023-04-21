#pragma once

#include <fmt/core.h>
#include <errors/compile_error.hpp>

namespace errors {
class ErrorHandler {
 public:
  static ErrorHandler& GetInstance() {
    static ErrorHandler handler;
    return handler;
  }

  void ReportCompileError(const errors::CompileError& error) {
    fmt::print("Error: {}\n", error.what());
  }

 private:
  ErrorHandler() = default;
};
} // namespace errors