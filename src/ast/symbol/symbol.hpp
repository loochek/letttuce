#pragma once

#include <string_view>
#include <ast/declarations.hpp>
#include <types/type.hpp>

namespace ast {

enum class SymbolType {
  Dummy,
  FnDecl,
  VarDecl,
  TypeDecl
};

struct VarSymbol {
  types::Type* type;
};

struct FnSymbol {
  types::FunctionType* type;
};

struct TypeSymbol {
  // in the future
};

struct Symbol {
  SymbolType type = SymbolType::Dummy;
  std::string_view name;
  lex::Location location;
  bool global_scope = false;

  std::variant<FnSymbol, VarSymbol, TypeSymbol> symbol;
};
} // namespace ast