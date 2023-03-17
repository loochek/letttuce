#pragma once

#include <string>

namespace error {

struct CompileError : std::exception {
  std::string message;

  const char* what() const noexcept override {
    return message.c_str();
  }
};
}  // namespace error
