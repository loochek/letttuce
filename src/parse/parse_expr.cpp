#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseExpression() {
  return ParseEqualityExpression();
}

///////////////////////////////////////////////////////////////////

Expression* Parser::ParseKeywordExpresssion() {
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

Expression* Parser::ParseIfExpression() {
  if (!Matches(lex::TokenType::IF))  {
    return nullptr;
  }

  lex::Token if_token = lexer_.GetPreviousToken();

  Expression *condition = ParseExpression();

  Consume(lex::TokenType::THEN);
  Expression *then_expr = ParseExpression();

  Expression *else_expr = nullptr;
  if (!Matches(lex::TokenType::ELSE))  {
    else_expr = ParseExpression();
  }

  return new IfExpression(if_token, condition, then_expr, else_expr);
}
////////////////////////////////////////////////////////////////////

Expression* Parser::ParseCompoundExpression() {
  if (!Matches(lex::TokenType::LEFT_BRACE)) {
    return nullptr;
  }

  std::vector<Statement*> statements;
  bool consumed = true;
  while (consumed) {
    consumed = false;
    try {
      Declaration* decl = ParseDeclaration();
      if (decl != nullptr) {
        statements.push_back(decl);
        consumed = true;
      }

      Statement* stmt = ParseStatement();
      if (stmt != nullptr) {
        statements.push_back(stmt);
        consumed = true;
      }
    } catch (parse::errors::ParseError& error) {
      ReportError(error.message);
      Synchronize();
    }

    if (!consumed) {
      break;
    }
  }

  // Try to match tail expression
  Expression* tail_expr = ParseExpression();

  Consume(lex::TokenType::RIGHT_BRACE);
  return new BlockExpression(std::move(statements), tail_expr);
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseEqualityExpression() {
  return ParseBinaryExpression<&Parser::ParseRelationalExpression, lex::TokenType::EQUALS, lex::TokenType::NOT_EQ>();
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseRelationalExpression() {
  return ParseBinaryExpression<&Parser::ParseAdditiveExpression, lex::TokenType::LT, lex::TokenType::GT>();
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseAdditiveExpression() {
  return ParseBinaryExpression<&Parser::ParseMultiplicativeExpression, lex::TokenType::PLUS, lex::TokenType::MINUS>();
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseMultiplicativeExpression() {
  return ParseBinaryExpression<&Parser::ParseUnaryExpression, lex::TokenType::STAR, lex::TokenType::DIV>();
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseUnaryExpression() {
  lex::Token token = lexer_.Peek();
  if (Matches(lex::TokenType::MINUS) || Matches(lex::TokenType::NOT)) {
    Expression* expr = ParseUnaryExpression();
    return new UnaryExpression(token, expr);
  }

  return ParsePostfixExpression();
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParsePostfixExpression() {
  return ParsePrimaryExpression();
  // TODO: multiple calls?
  // TODO: fix

//  lex::Token fn_name = lexer_.Peek();
//  if (!Matches(lex::TokenType::IDENTIFIER)) {
//    return ParseExpression();
//  } else {
//
//  }
//
//  // Function call branch
//
//  if (Matches(lex::TokenType::RIGHT_CBRACE)) {
//    // No arguments
//    return new FnCallExpression(lhs, std::vector<Expression*>{});
//  }
//
//  std::vector<Expression*> args;
//
//  // First argument
//  args.push_back(ParseExpression());
//  while (!Matches(lex::TokenType::RIGHT_CBRACE)) {
//    Consume(lex::TokenType::COMMA);
//    args.push_back(ParseExpression());
//  }
//
//  return new FnCallExpression(lhs, std::move(args));
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParsePrimaryExpression() {
  // Try parsing grouping first

  if (Matches(lex::TokenType::LEFT_CBRACE)) {
    Expression *expr = ParseExpression();
    Consume(lex::TokenType::RIGHT_CBRACE);
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
    case lex::TokenType::TRUE:
    case lex::TokenType::FALSE:
      lexer_.Advance();
      return new LiteralExpression(curr_token);

    default:
      throw parse::errors::ParsePrimaryError(curr_token.location.Format());
  }

  FMT_ASSERT(false, "Unreachable!");
}

////////////////////////////////////////////////////////////////////

Expression* Parser::ParseReturnExpression() {
  if (!Matches(lex::TokenType::RETURN)) {
    return nullptr;
  }

  lex::Token return_token = lexer_.GetPreviousToken();

  Expression* expr = ParseExpression();
  return new ReturnExpression(return_token, expr);
}

///////////////////////////////////////////////////////////////////

Expression* Parser::ParseYieldExpression() {
  if (!Matches(lex::TokenType::YIELD)) {
    return nullptr;
  }

  lex::Token yield_token = lexer_.GetPreviousToken();

  Expression* expr = ParseExpression();
  return new YieldExpression(yield_token, expr);
}

///////////////////////////////////////////////////////////////////
