#include <lex/lexer.hpp>

namespace lex {

Lexer::Lexer(std::istream& source) : scanner_{source} {
  Advance();
}

////////////////////////////////////////////////////////////////////

Token Lexer::GetNextToken() {
  SkipWhitespace();
  SkipComments();

  if (auto op = MatchOperators()) {
    return *op;
  }

  if (auto lit = MatchLiterals()) {
    return *lit;
  }

  if (auto word = MatchWords()) {
    return *word;
  }

  FMT_ASSERT(false, "Could not match any token\n");
}

////////////////////////////////////////////////////////////////////

Token Lexer::GetPreviousToken() {
  return prev_;
}

////////////////////////////////////////////////////////////////////

void Lexer::Advance() {
  prev_ = peek_;
  peek_ = GetNextToken();
}

////////////////////////////////////////////////////////////////////

bool Lexer::Matches(lex::TokenType type) {
  if (Peek().type == type) {
    Advance();
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////

Token Lexer::Peek() {
  return peek_;
}

////////////////////////////////////////////////////////////////////

bool IsWhitespace(char ch) {
  return ch == ' ' || ch == '\n' || ch == '\t';
}

void Lexer::SkipWhitespace() {
  while (IsWhitespace(scanner_.CurrentSymbol())) {
    scanner_.MoveNext();
  }
}

////////////////////////////////////////////////////////////////////

void Lexer::SkipComments() {
  while (scanner_.CurrentSymbol() == '#') {
    scanner_.MoveNextLine();
    SkipWhitespace();
  }
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchOperators() {
  // Save location first
  Location location = scanner_.GetLocation();
  auto op_type = PeekOperator();
  if (op_type.has_value()) {
    // All operators are single-symbol except != and ==
    switch (*op_type) {
      case TokenType::NOT_EQ:
      case TokenType::EQUALS:
        scanner_.MoveNext();
        [[fallthrough]];
      default:
        scanner_.MoveNext();
    }

    return Token(*op_type, location);
  }

  return std::nullopt;
}

////////////////////////////////////////////////////////////////////

std::optional<TokenType> Lexer::PeekOperator() {
  switch (scanner_.CurrentSymbol()) {
    case '+':
      return TokenType::PLUS;

    case '-':
      return TokenType::MINUS;

    case '*':
      return TokenType::STAR;

    case '/':
      return TokenType::DIV;

    case '<':
      return TokenType::LT;

    case '>':
      return TokenType::GT;

    case '(':
      return TokenType::LEFT_BRACE;

    case ')':
      return TokenType::RIGHT_BRACE;

    case '{':
      return TokenType::LEFT_CBRACE;

    case '}':
      return TokenType::RIGHT_CBRACE;

    case ',':
      return TokenType::COMMA;

    case ';':
      return TokenType::SEMICOLON;

    case ':':
      return TokenType::COLUMN;

    case EOF:
      return TokenType::TOKEN_EOF;

    case '=':
      if (scanner_.NextSymbol() == '=') {
        return TokenType::EQUALS;
      } else {
        return TokenType::ASSIGN;
      }

    case '!':
      if (scanner_.NextSymbol() == '=') {
        return TokenType::NOT_EQ;
      } else {
        return TokenType::NOT;
      }

    default:
      return std::nullopt;
  }
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchLiterals() {
  if (auto num = MatchNumericLiteral()) {
    return num;
  }

  if (auto str = MatchStringLiteral()) {
    return str;
  }

  return std::nullopt;
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchNumericLiteral() {
  if (!isdigit(scanner_.CurrentSymbol())) {
    return std::nullopt;
  }

  Location start_loc = scanner_.GetLocation();

  int literal = 0;
  while (isdigit(scanner_.CurrentSymbol())) {
    literal *= 10;
    literal += scanner_.CurrentSymbol() - '0';
    scanner_.MoveNext();
  }

  return Token(TokenType::NUMBER, start_loc, literal);
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchStringLiteral() {
  Location start_loc = scanner_.GetLocation();
  if (scanner_.CurrentSymbol() != '"') {
    return std::nullopt;
  }

  scanner_.MoveNext();
  MoveNextWhileCond([&]() {
    return scanner_.CurrentSymbol() != '"';
  });

  if (scanner_.CurrentSymbol() == EOF) {
    FMT_ASSERT(false, "Unexpected end of the string literal");
  } else {
    Location end_loc = scanner_.GetLocation();
    scanner_.MoveNext();
    return Token(TokenType::STRING, start_loc, scanner_.GetSlice(start_loc, end_loc));
  }
}

////////////////////////////////////////////////////////////////////

std::optional<Token> Lexer::MatchWords() {
  // Identifier or some keyword - starting with alpha
  if (!isalpha(scanner_.CurrentSymbol())) {
    return std::nullopt;
  }

  Location start_loc = scanner_.GetLocation();

  scanner_.MoveNext();
  MoveNextWhileCond([&]() {
    return isalpha(scanner_.CurrentSymbol()) ||
           isdigit(scanner_.CurrentSymbol());
  });

  Location end_loc = scanner_.GetLocation();
  std::string_view word = scanner_.GetSlice(start_loc, end_loc);

  // The word would be a keyword if it matches with some template, and identifier otherwise
  TokenType word_type = table_.LookupWord(word);
  return Token(word_type, start_loc, word);
}

}  // namespace lex
