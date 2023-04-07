#pragma once

#include <unordered_map>
#include <ast/symbol/symbol.hpp>

namespace ast {

class Scope {
 public:
  Scope(lex::Location location, Scope* parent = nullptr) : location_(location), parent_(parent) {
  }

  Scope* GetParent() const {
    return parent_;
  }

  lex::Location GetLocation() const {
    return location_;
  }

  bool AddSymbol(Symbol symbol) {
    if (symbols_.find(symbol.name) != symbols_.end()) {
      return false;
    }

    symbols_[symbol.name] = symbol;
    return true;
  }

  Symbol* LookupLocal(const std::string_view& name, const lex::Location& location) {
    for (auto& [symbol_name, symbol] : symbols_) {
      if (symbol_name == name && (symbol.global_scope || (symbol.location < location))) {
        return &symbol;
      }
    }

    return nullptr;
  }

  Symbol* Lookup(const std::string_view& name, const lex::Location& location) {
    Symbol* local = LookupLocal(name, location);
    if (local != nullptr) {
      return local;
    }

    // If symbol is not present locally, search it in parent scopes
    return parent_ != nullptr ? GetParent()->Lookup(name, location) : nullptr;
  }

 private:
  std::unordered_map<std::string_view, Symbol> symbols_;
  lex::Location location_;
  Scope* parent_;


};
} // namespace ast