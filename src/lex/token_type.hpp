#pragma once

#include <fmt/core.h>
#include <cstdlib>

namespace lex {
//////////////////////////////////////////////////////////////////////

enum class TokenType {
  DUMMY,
  NUMBER,
  STRING,
  IDENTIFIER,
  TRUE,
  FALSE,
  PLUS,
  MINUS,
  STAR,
  DIV,
  ASSIGN,
  EQUALS,
  NOT_EQ,
  NOT,
  LT,
  GT,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_CBRACE,
  RIGHT_CBRACE,
  LEFT_SBRACE,
  RIGHT_SBRACE,
  COMMA,
  COLUMN,
  SEMICOLON,
  ARROW,
  FUN,
  VAR,
  IF,
  THEN,
  ELSE,
  FOR,
  RETURN,
  YIELD,
  OF,

  TY_INT,
  TY_BOOL,
  TY_STRING,
  TY_UNIT,

  TOKEN_EOF
};

////////////////////////////////////////////////////////////////

inline const char* FormatTokenType(TokenType type) {
  switch (type) {
    case TokenType::DUMMY:
      return "<DUMMY>";
    case TokenType::NUMBER:
      return "<NUMER>";
    case TokenType::STRING:
      return "<STRING>";
    case TokenType::IDENTIFIER:
      return "<IDENTIFIER>";
    case TokenType::TRUE:
      return "true";
    case TokenType::FALSE:
      return "false";
    case TokenType::PLUS:
      return "+";
    case TokenType::MINUS:
      return "-";
    case TokenType::STAR:
      return "*";
    case TokenType::DIV:
      return "/";
    case TokenType::ASSIGN:
      return "=";
    case TokenType::EQUALS:
      return "==";
    case TokenType::NOT_EQ:
      return "!=";
    case TokenType::NOT:
      return "!";
    case TokenType::LT:
      return "<";
    case TokenType::GT:
      return ">";
    case TokenType::LEFT_CBRACE:
      return "{";
    case TokenType::RIGHT_CBRACE:
      return "}";
    case TokenType::LEFT_BRACE:
      return "(";
    case TokenType::RIGHT_BRACE:
      return ")";
    case TokenType::COMMA:
      return ",";
    case TokenType::COLUMN:
      return ":";
    case TokenType::SEMICOLON:
      return ";";
    case TokenType::FUN:
      return "fun";
    case TokenType::VAR:
      return "var";
    case TokenType::IF:
      return "if";
    case TokenType::THEN:
      return "then";
    case TokenType::ELSE:
      return "else";
    case TokenType::FOR:
      return "for";
    case TokenType::RETURN:
      return "return";
    case TokenType::YIELD:
      return "yield";
    case TokenType::TY_INT:
      return "Int";
    case TokenType::TY_BOOL:
      return "Bool";
    case TokenType::TY_STRING:
      return "String";
    case TokenType::TY_UNIT:
      return "Unit";
    case TokenType::TOKEN_EOF:
      return "<EOF>";
    default:
      FMT_ASSERT(false, "Unknown token type");
  }
}

////////////////////////////////////////////////////////////////

}  // namespace lex
