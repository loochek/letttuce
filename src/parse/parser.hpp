#pragma once

#include <ast/declarations.hpp>
#include <parse/parse_error.hpp>
#include <lex/lexer.hpp>
#include <utility>

class Parser {
 public:
  explicit Parser(lex::Lexer& l);

  Program* ParseProgram();

  ///////////////////////////////////////////////////////////////////

  Statement* ParseStatement();

  ////////////////////////////////////////////////////////////////////

  Declaration* ParseDeclaration();

  FunDeclStatement* ParseFunDeclStatement();
  VarDeclStatement* ParseVarDeclStatement();

  ////////////////////////////////////////////////////////////////////

  Expression* ParseExpression();

  Expression* ParseKeywordExpresssion();

  Expression* ParseReturnExpression();
  Expression* ParseYieldExpression();
  Expression* ParseIfExpression();

  Expression* ParseCompoundExpression();

  // Precedences
  Expression* ParseEqualityExpression();
  Expression* ParseRelationalExpression();
  Expression* ParseAdditiveExpression();
  Expression* ParseMultiplicativeExpression();
  Expression* ParsePostfixExpression();
  Expression* ParseUnaryExpression();
  Expression* ParsePrimaryExpression();

  ////////////////////////////////////////////////////////////////////

  template<Expression* (Parser::*InnerParser)(), lex::TokenType... Tokens>
  Expression* ParseBinaryExpression() {
    Expression *lhs = (this->*InnerParser)();

    while ((Matches(Tokens) || ...)) {
      lex::Token operation = lexer_.GetPreviousToken();
      lhs = new BinaryExpression(operation, lhs, (this->*InnerParser)());
    }

    return lhs;
  }

  ////////////////////////////////////////////////////////////////////

 private:
  std::string FormatLocation() {
    return lexer_.Peek().location.Format();
  }

  std::vector<lex::Token> ParseFunctionArgs();

  bool Matches(lex::TokenType type) {
    return lexer_.Matches(type);
  }

  void Consume(lex::TokenType type);
  void ReportError(const parse::errors::ParseError& error);

  /// Skips tokens until semicolon or EOF is encountered
  void Synchronize();

 private:
  lex::Lexer& lexer_;
};
