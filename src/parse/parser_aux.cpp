#include <parse/parse_error.hpp>
#include <parse/parser.hpp>

parse::Parser::Parser(lex::Lexer& l) : lexer_{l} {
}

void parse::Parser::Consume(lex::TokenType type) {
  if (!Matches(type)) {
    throw parse::errors::ParseTokenError(lex::FormatTokenType(type), FormatLocation());
  }
}

void parse::Parser::ReportError(const parse::errors::ParseError& error) {
  // TODO: normal error printing
  if (dynamic_cast<const parse::errors::ParseCompoundError*>(&error) != nullptr) {
    // ew
    return;
  }

  fmt::print("Error: {}\n", error.what());
}

void parse::Parser::Synchronize() {
  lexer_.Advance();

  while (lexer_.Peek().type != lex::TokenType::TOKEN_EOF) {
    if (lexer_.Matches(lex::TokenType::SEMICOLON)) {
      return;
    }

    lexer_.Advance();
  }
}