#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

///////////////////////////////////////////////////////////////////

ast::Program* parse::Parser::ParseProgram() {
  bool errors_occured = false;
  std::vector<ast::Declaration*> decls;
  while (!lexer_.Matches(lex::TokenType::TOKEN_EOF)) {
    try {
      ast::Declaration* decl = ParseDeclaration();
      if (decl == nullptr) {
        throw parse::errors::ParseDeclarationError((lexer_.Peek().location.Format()));
      }

      decls.push_back(decl);
    } catch (parse::errors::ParseError& error) {
      ReportError(error);
      Synchronize();
      errors_occured = true;
    }
  }

  if (errors_occured) {
    throw parse::errors::ParseProgramError();
  }

  return new ast::Program(std::move(decls));
}

///////////////////////////////////////////////////////////////////

ast::Declaration* parse::Parser::ParseDeclaration() {
  if (auto var_declaration = ParseVarDeclStatement()) {
    return var_declaration;
  }

  if (auto fun_declaration = ParseFunDeclStatement()) {
    return fun_declaration;
  }

  return nullptr;
}

///////////////////////////////////////////////////////////////////

ast::FunDeclStatement* parse::Parser::ParseFunDeclStatement() {
  if (!Matches(lex::TokenType::FUN)) {
    return nullptr;
  }

  lex::Token fun_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::LEFT_BRACE);
  auto args = ParseFunctionArgs();
  Consume(lex::TokenType::RIGHT_BRACE);
  Consume(lex::TokenType::ASSIGN);
  ast::Expression* body = ParseExpression();
  Consume(lex::TokenType::SEMICOLON);
  return new ast::FunDeclStatement(fun_name, std::move(args), body);
}

///////////////////////////////////////////////////////////////////

std::vector<lex::Token> parse::Parser::ParseFunctionArgs() {
  std::vector<lex::Token> args;

  while (Matches(lex::TokenType::IDENTIFIER)) {
    args.push_back(lexer_.GetPreviousToken());
    if (!Matches(lex::TokenType::COMMA)) {
      break;
    }
  }

  return args;
}

///////////////////////////////////////////////////////////////////

ast::VarDeclStatement* parse::Parser::ParseVarDeclStatement() {
  if (!Matches(lex::TokenType::VAR)) {
    return nullptr;
  }

  lex::Token var_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::ASSIGN);
  ast::Expression* value = ParseExpression();
  Consume(lex::TokenType::SEMICOLON);
  return new ast::VarDeclStatement(var_name, value);
}

///////////////////////////////////////////////////////////////////
