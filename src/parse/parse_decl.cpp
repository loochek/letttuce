#include <parse/parser.hpp>
#include <parse/parse_error.hpp>

///////////////////////////////////////////////////////////////////

Program* Parser::ParseProgram() {
  bool errors_occured = false;
  std::vector<Declaration*> decls;
  while (!lexer_.Matches(lex::TokenType::TOKEN_EOF)) {
    try {
      Declaration* decl = ParseDeclaration();
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

  return new Program(std::move(decls));
}

///////////////////////////////////////////////////////////////////

Declaration* Parser::ParseDeclaration() {
  if (auto var_declaration = ParseVarDeclStatement()) {
    return var_declaration;
  }

  if (auto fun_declaration = ParseFunDeclStatement()) {
    return fun_declaration;
  }

  return nullptr;
}

///////////////////////////////////////////////////////////////////

FunDeclStatement* Parser::ParseFunDeclStatement() {
  if (!Matches(lex::TokenType::FUN)) {
    return nullptr;
  }

  lex::Token fun_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::LEFT_BRACE);
  auto args = ParseFunctionArgs();
  Consume(lex::TokenType::RIGHT_BRACE);
  Consume(lex::TokenType::ASSIGN);
  Expression* body = ParseExpression();
  Consume(lex::TokenType::SEMICOLON);
  return new FunDeclStatement(fun_name, std::move(args), body);
}

///////////////////////////////////////////////////////////////////

std::vector<lex::Token> Parser::ParseFunctionArgs() {
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

VarDeclStatement* Parser::ParseVarDeclStatement() {
  if (!Matches(lex::TokenType::VAR)) {
    return nullptr;
  }

  lex::Token var_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::ASSIGN);
  Expression* value = ParseExpression();
  Consume(lex::TokenType::SEMICOLON);
  return new VarDeclStatement(var_name, value);
}

///////////////////////////////////////////////////////////////////
