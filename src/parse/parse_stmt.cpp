#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

///////////////////////////////////////////////////////////////////

ast::Statement* parse::Parser::ParseStatement() {
  ast::Expression* expr = ParseExpression();

  if (Matches(lex::TokenType::ASSIGN)) {
    // Assignment statement
    lex::Token assn_token = lexer_.GetPreviousToken();
    ast::Expression* value = ParseExpression();
    Consume(lex::TokenType::SEMICOLON);
    return new ast::AssignmentStatement(assn_token, expr->as<ast::LvalueExpression>(), value);
  }

  // Expression statement
  Consume(lex::TokenType::SEMICOLON);
  return new ast::ExprStatement(expr);
}