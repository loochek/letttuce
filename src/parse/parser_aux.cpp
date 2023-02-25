#include <parse/parse_error.hpp>
#include <parse/parser.hpp>

Parser::Parser(lex::Lexer& l) : lexer_{l} {
}

void Parser::Consume(lex::TokenType type) {
  if (!Matches(type)) {
    throw parse::errors::ParseTokenError(lex::FormatTokenType(type), FormatLocation());
  }
}

void Parser::ReportError(const std::string& message) {
  // TODO: normal error printing
  fmt::print("Error at {}: {}\n", lexer_.Peek().location.Format(), message);
}

void Parser::Synchronize() {
  lexer_.Advance();

  while (lexer_.Peek().type != lex::TokenType::TOKEN_EOF) {
    if (lexer_.Matches(lex::TokenType::SEMICOLON)) {
      return;
    }

    lexer_.Advance();
  }
}