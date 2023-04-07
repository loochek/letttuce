#pragma once

#include <types/type.hpp>
#include <lex/token_type.hpp>

namespace types {
class PrimitiveType : public Type {
 public:
  PrimitiveType() = delete;

  std::string Format() const override {
    return FormatTokenType(type_);
  }

  bool Equals(types::Type* other) const override {
    if (auto other_prim = dynamic_cast<PrimitiveType*>(other)) {
      return other_prim->type_ == type_;
    }

    return false;
  }

 private:
  explicit PrimitiveType(lex::TokenType type) : type_(type) {}

 public:
  static PrimitiveType int_type;
  static PrimitiveType bool_type;
  static PrimitiveType string_type;
  static PrimitiveType unit_type;

 private:
  lex::TokenType type_;
};

inline PrimitiveType PrimitiveType::int_type(lex::TokenType::TY_INT);
inline PrimitiveType PrimitiveType::bool_type(lex::TokenType::TY_BOOL);
inline PrimitiveType PrimitiveType::string_type(lex::TokenType::TY_STRING);
inline PrimitiveType PrimitiveType::unit_type(lex::TokenType::TY_UNIT);
}