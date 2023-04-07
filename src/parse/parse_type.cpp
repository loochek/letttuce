#include <parse/parser.hpp>
#include <types/primitive_types.hpp>

types::Type* parse::Parser::ParsePrimitiveType() {
  lex::Token next_token = lexer_.Peek();
  switch (next_token.type) {
    case lex::TokenType::TY_UNIT:
      lexer_.Advance();
      return &types::PrimitiveType::unit_type;

    case lex::TokenType::TY_INT:
      lexer_.Advance();
      return &types::PrimitiveType::int_type;

    case lex::TokenType::TY_STRING:
      lexer_.Advance();
      return &types::PrimitiveType::string_type;

    case lex::TokenType::TY_BOOL:
      lexer_.Advance();
      return &types::PrimitiveType::bool_type;

    case lex::TokenType::NOT:
      lexer_.Advance();
      return nullptr; // ???

    case lex::TokenType::LEFT_BRACE:
      lexer_.Advance();
      Consume(lex::TokenType::RIGHT_BRACE);
      return nullptr; // ???

    default:
      throw parse::error::ParseTypeError(next_token.location.Format());
  }
}

types::Type* parse::Parser::ParseType() {
  if (!lexer_.Matches(lex::TokenType::LEFT_SBRACE)) {
    return ParseSimpleType();
  }

  std::vector<types::Type*> args;
  if (!Matches(lex::TokenType::RIGHT_SBRACE)) {
    args.push_back(ParseType());
    while (!Matches(lex::TokenType::RIGHT_SBRACE)) {
      Consume(lex::TokenType::COMMA);
      args.push_back(ParseType());
    }
  }

  Consume(lex::TokenType::ARROW);
  types::Type *return_type = ParseType();
  return type_keeper_.CreateType<types::FunctionType>(return_type, std::move(args));
}

types::Type* parse::Parser::ParseSimpleType() {
  if (lexer_.Matches(lex::TokenType::STAR)) {
    return type_keeper_.CreateType<types::PointerType>(ParseSimpleType());
  }

  return ParsePrimitiveType();
}

types::Type* parse::Parser::ParseSignature() {
  if (!lexer_.Matches(lex::TokenType::OF)) {
    return nullptr;
  }

  return ParseType();
}