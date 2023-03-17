#pragma once

#include <string_view>
#include <ast/declarations.hpp>

namespace ast {

enum class SymbolType {
  Dummy,
  FnDecl,
  VarDecl,
  TypeDecl
};

struct VarSymbol {
  // Type? (in the future)
};

struct FnSymbol {
  // Types of arguments and return value ???
};

struct TypeSymbol {
  // in the future
};

struct Symbol {
  SymbolType type = SymbolType::Dummy;
  std::string_view name;
  lex::Location location;

  std::variant<FnSymbol, VarSymbol, TypeSymbol> symbol;
};
} // namespace ast