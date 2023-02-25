#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

///////////////////////////////////////////////////////////////////

Statement* Parser::ParseStatement() {
  Expression* expr = ParseExpression();

  if (Matches(lex::TokenType::ASSIGN)) {
    // Assignment statement
    lex::Token assn_token = lexer_.GetPreviousToken();
    Expression* value = ParseExpression();
    Consume(lex::TokenType::SEMICOLON);
    return new AssignmentStatement(assn_token, expr->as<LvalueExpression>(), value);
  }

  // Expression statement
  Consume(lex::TokenType::SEMICOLON);
  return new ExprStatement(expr);
}