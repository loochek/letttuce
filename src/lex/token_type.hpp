#pragma once

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
  COMMA,
  COLUMN,
  SEMICOLON,
  FUN,
  VAR,
  IF,
  THEN,
  ELSE,
  FOR,
  RETURN,
  YIELD,

  TY_INT,
  TY_BOOL,
  TY_STRING,
  TY_UNIT,

  TOKEN_EOF
};

////////////////////////////////////////////////////////////////

const char* FormatTokenType(TokenType type);

////////////////////////////////////////////////////////////////

}  // namespace lex
