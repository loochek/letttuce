#pragma once

#include <fmt/core.h>
#include <errors/compile_error.hpp>

namespace ast::errors {

struct SymbolError : ::errors::CompileError {};

struct RedefinitionError : SymbolError {
  explicit RedefinitionError(const std::string_view symbol_name, const std::string& location) {
    message =
        fmt::format("Redefinition of symbol {} (already defined in current scope) at {}", symbol_name, location);
  }
};

struct UndefinedSymbolError : SymbolError {
  explicit UndefinedSymbolError(const std::string_view symbol_name, const std::string& location) {
    message =
        fmt::format("Use of undefined symbol {} at {}", symbol_name, location);
  }
};
}