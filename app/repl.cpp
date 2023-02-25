#include <fmt/color.h>
#include <ast/expressions.hpp>
#include <ast/statements.hpp>
#include <ast/declarations.hpp>
#include <ast/visitors/print_visitor.hpp>

#include <iostream>

int main() {
  auto* str_decl = new VarDeclStatement{
      lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "str"},
      new LiteralExpression{
          lex::Token{lex::TokenType::STRING, lex::Location{}, "Hello!"}}};

  auto* printf_call = new FnCallExpression{
      lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "printf"},
      std::vector<Expression*>{
          new LiteralExpression{
              lex::Token{lex::TokenType::STRING, lex::Location{}, "%s"}},
          new VarAccessExpression{
              lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "str"}}}};

  auto* comp_stmt = new BlockExpression{
      std::vector<Statement*>{str_decl, new ExprStatement{printf_call}}, nullptr};

  auto* fn_decl = new FunDeclStatement{
      lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "aboba"},
      std::vector{
          lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "argc"},
          lex::Token{lex::TokenType::IDENTIFIER, lex::Location{}, "argv"}},
      comp_stmt};

  PrintVisitor print_visitor;
  fn_decl->Accept(&print_visitor);
  return 0;
}
