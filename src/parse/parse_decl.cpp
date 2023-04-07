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
        throw parse::error::ParseDeclarationError((lexer_.Peek().location.Format()));
      }

      decls.push_back(decl);
    } catch (parse::error::ParseError& error) {
      ReportError(error);
      Synchronize();
      errors_occured = true;
    }
  }

  if (errors_occured) {
    throw parse::error::ParseProgramError();
  }

  return new ast::Program(std::move(decls));
}

///////////////////////////////////////////////////////////////////

ast::Declaration* parse::Parser::ParseDeclaration() {
  types::Type* type = ParseSignature();
  if (type == nullptr) {
    return nullptr;
  }

  if (auto var_declaration = ParseVarDeclStatement(type)) {
    return var_declaration;
  }

  if (auto fun_declaration = ParseFunDeclStatement(type)) {
    return fun_declaration;
  }

  throw parse::error::ParseDeclarationError(lexer_.Peek().location.Format());
}

///////////////////////////////////////////////////////////////////

ast::FunDeclStatement* parse::Parser::ParseFunDeclStatement(types::Type* type) {
  lex::Location location = lexer_.Peek().location;
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

  auto func_type = dynamic_cast<types::FunctionType*>(type);
  if (func_type == nullptr) {
    func_type = type_keeper_.CreateType<types::FunctionType>(type, std::vector<types::Type*>{});
  }

  // TODO: move this checks to the separate pass?
  if (func_type->GetArgTypes().size() != args.size()) {
    // Amount of arguments doesn't match with specified function type
    throw parse::error::FnDeclArgsCountMismatchError(location.Format());
  }

  return new ast::FunDeclStatement(fun_name, std::move(args), func_type, body);
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

ast::VarDeclStatement* parse::Parser::ParseVarDeclStatement(types::Type* type) {
  if (!Matches(lex::TokenType::VAR)) {
    return nullptr;
  }

  lex::Token var_name = lexer_.Peek();
  Consume(lex::TokenType::IDENTIFIER);
  Consume(lex::TokenType::ASSIGN);
  ast::Expression* value = ParseExpression();
  Consume(lex::TokenType::SEMICOLON);
  return new ast::VarDeclStatement(var_name, type, value);
}

///////////////////////////////////////////////////////////////////
