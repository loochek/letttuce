#pragma once

#include <ast/declarations.hpp>
#include <types/type.hpp>
#include <parse/parse_error.hpp>
#include <lex/lexer.hpp>
#include <utils/storage.hpp>
#include <utility>

// TODO: handle lifetime of scopes and AST nodes

namespace parse {
class Parser {
 public:
  explicit Parser(lex::Lexer& lexer, utils::Storage<types::Type>& type_keeper);

  ast::Program* ParseProgram();

  ///////////////////////////////////////////////////////////////////

  ast::Statement* ParseStatement();

  ////////////////////////////////////////////////////////////////////

  ast::Declaration* ParseDeclaration();

  ast::FunDeclStatement* ParseFunDeclStatement(types::Type* type);
  ast::VarDeclStatement* ParseVarDeclStatement(types::Type* type);

  ////////////////////////////////////////////////////////////////////

  ast::Expression* ParseExpression();

  ast::Expression* ParseKeywordExpresssion();

  ast::Expression* ParseReturnExpression();
  ast::Expression* ParseYieldExpression();
  ast::Expression* ParseIfExpression();

  ast::Expression* ParseCompoundExpression();

  // Precedences
  ast::Expression* ParseEqualityExpression();
  ast::Expression* ParseRelationalExpression();
  ast::Expression* ParseAdditiveExpression();
  ast::Expression* ParseMultiplicativeExpression();
  ast::Expression* ParsePostfixExpression();
  ast::Expression* ParseUnaryExpression();
  ast::Expression* ParsePrimaryExpression();

  ////////////////////////////////////////////////////////////////////

  template<
      ast::Expression* (Parser::*InnerParser)(),
      typename ResultNode,
      lex::TokenType... Tokens
      >
  ast::Expression* ParseBinaryExpression() {
    ast::Expression *lhs = (this->*InnerParser)();

    while ((Matches(Tokens) || ...)) {
      lex::Token operation = lexer_.GetPreviousToken();
      lhs = new ResultNode(operation, lhs, (this->*InnerParser)());
    }

    return lhs;
  }

  ////////////////////////////////////////////////////////////////////

  types::Type* ParsePrimitiveType();
  types::Type* ParseSimpleType();
  types::Type* ParseType();
  types::Type* ParseSignature();


 private:
  std::string FormatLocation() {
    return lexer_.Peek().location.Format();
  }

  std::vector<lex::Token> ParseFunctionArgs();

  bool Matches(lex::TokenType type) {
    return lexer_.Matches(type);
  }

  void Consume(lex::TokenType type);
  static void ReportError(const errors::ParseError& error);

  /// Skips tokens until semicolon or EOF is encountered
  void Synchronize();

 private:
  lex::Lexer& lexer_;
  utils::Storage<types::Type>& type_keeper_;
};
}  // namespace parse
