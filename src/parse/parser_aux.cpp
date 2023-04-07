#include <parse/parse_error.hpp>
#include <parse/parser.hpp>
#include <error/error_handler.hpp>

parse::Parser::Parser(lex::Lexer& lexer, utils::Storage<types::Type>& type_keeper) :
      lexer_{lexer}, type_keeper_{type_keeper} {
}

void parse::Parser::Consume(lex::TokenType type) {
  if (!Matches(type)) {
    throw parse::error::ParseTokenError(lex::FormatTokenType(type), FormatLocation());
  }
}

void parse::Parser::ReportError(const parse::error::ParseError& error) {
  // TODO: normal error printing
  if (dynamic_cast<const parse::error::ParseCompoundError*>(&error) != nullptr) {
    // ew
    return;
  }

  ::error::ErrorHandler::GetInstance().ReportCompileError(error);
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
