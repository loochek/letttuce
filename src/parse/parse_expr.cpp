#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseExpression() {
  return ParseEqualityExpression();
}

///////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseKeywordExpresssion() {
  if (auto return_expr = ParseReturnExpression()) {
    return return_expr;
  }

  if (auto yield_expr = ParseYieldExpression()) {
    return yield_expr;
  }

  if (auto if_expr = ParseIfExpression()) {
    return if_expr;
  }

  return nullptr;
}

///////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseIfExpression() {
  if (!Matches(lex::TokenType::IF))  {
    return nullptr;
  }

  lex::Token if_token = lexer_.GetPreviousToken();

  ast::Expression *condition = ParseExpression();

  Consume(lex::TokenType::THEN);
  ast::Expression *then_expr = ParseExpression();

  ast::Expression *else_expr = nullptr;
  if (Matches(lex::TokenType::ELSE))  {
    else_expr = ParseExpression();
  }

  return new ast::IfExpression(if_token, condition, then_expr, else_expr);
}
////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseCompoundExpression() {
  if (!Matches(lex::TokenType::LEFT_CBRACE)) {
    return nullptr;
  }

  lex::Token compound_start_token = lexer_.GetPreviousToken();

  bool errors_occured = false;
  std::vector<ast::Statement*> statements;
  while (!lexer_.Matches(lex::TokenType::RIGHT_CBRACE)) {
    try {
      ast::Declaration* decl = ParseDeclaration();
      if (decl != nullptr) {
        statements.push_back(decl);
      } else {
        statements.push_back(ParseStatement());
      }
    } catch (parse::error::ParseError& error) {
      ReportError(error);
      Synchronize();
      errors_occured = true;
    }
  }

  if (errors_occured) {
    throw parse::error::ParseCompoundError(compound_start_token.location.Format());
  }

  return new ast::BlockExpression(std::move(statements));
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseEqualityExpression() {
  return ParseBinaryExpression<
      &Parser::ParseRelationalExpression,
      ast::ComparisonExpression,
      lex::TokenType::EQUALS, lex::TokenType::NOT_EQ
      >();
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseRelationalExpression() {
  return ParseBinaryExpression<
      &Parser::ParseAdditiveExpression,
      ast::ComparisonExpression,
      lex::TokenType::LT, lex::TokenType::GT
      >();
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseAdditiveExpression() {
  return ParseBinaryExpression<
      &Parser::ParseMultiplicativeExpression,
      ast::BinaryExpression,
      lex::TokenType::PLUS, lex::TokenType::MINUS
      >();
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseMultiplicativeExpression() {
  return ParseBinaryExpression<
      &parse::Parser::ParseUnaryExpression,
      ast::BinaryExpression,
      lex::TokenType::STAR, lex::TokenType::DIV
      >();
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseUnaryExpression() {
  lex::Token token = lexer_.Peek();
  if (Matches(lex::TokenType::MINUS) || Matches(lex::TokenType::NOT)) {
    ast::Expression* expr = ParseUnaryExpression();
    return new ast::UnaryExpression(token, expr);
  }

  return ParsePostfixExpression();
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParsePostfixExpression() {
  ast::Expression* callable = ParsePrimaryExpression();

  while (Matches(lex::TokenType::LEFT_BRACE)) {
    if (Matches(lex::TokenType::RIGHT_BRACE)) {
      // No arguments
      callable = new ast::FnCallExpression(callable, std::vector<ast::Expression*>{});
      continue;
    }

    std::vector<ast::Expression*> args;
    // First argument
    args.push_back(ParseExpression());
    while (!Matches(lex::TokenType::RIGHT_BRACE)) {
      Consume(lex::TokenType::COMMA);
      args.push_back(ParseExpression());
    }

    callable = new ast::FnCallExpression(callable, std::move(args));
  }

  return callable;
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParsePrimaryExpression() {
  // Try parsing grouping first

  if (Matches(lex::TokenType::LEFT_BRACE)) {
    ast::Expression *expr = ParseExpression();
    Consume(lex::TokenType::RIGHT_BRACE);
    return expr;
  }

  if (auto comp_expr = ParseCompoundExpression()) {
    return comp_expr;
  }

  // Then keyword expressions
  if (auto kw_expr = ParseKeywordExpresssion()) {
    return kw_expr;
  }

  // Then all the base cases: IDENT, INT, TRUE, FALSE, ETC...

  lex::Token curr_token = lexer_.Peek();
  switch (curr_token.type) {
    case lex::TokenType::IDENTIFIER:
    case lex::TokenType::NUMBER:
    case lex::TokenType::STRING:
    case lex::TokenType::TRUE:
    case lex::TokenType::FALSE:
      lexer_.Advance();
      return new ast::LiteralExpression(curr_token);

    default:
      throw parse::error::ParsePrimaryError(curr_token.location.Format());
  }

  FMT_ASSERT(false, "Unreachable!");
}

////////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseReturnExpression() {
  if (!Matches(lex::TokenType::RETURN)) {
    return nullptr;
  }

  lex::Token return_token = lexer_.GetPreviousToken();

  ast::Expression* expr = ParseExpression();
  return new ast::ReturnExpression(return_token, expr);
}

///////////////////////////////////////////////////////////////////

ast::Expression* parse::Parser::ParseYieldExpression() {
  if (!Matches(lex::TokenType::YIELD)) {
    return nullptr;
  }

  lex::Token yield_token = lexer_.GetPreviousToken();

  ast::Expression* expr = ParseExpression();
  return new ast::YieldExpression(yield_token, expr);
}

///////////////////////////////////////////////////////////////////
