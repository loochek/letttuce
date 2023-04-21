#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/base_visitor.hpp>
#include <ast/symbol/symbol_error.hpp>

namespace passes {
/// Builds symbol table and checks for definitions
class DefinitionChecker : public ast::BaseVisitor {
 public:
  void VisitLiteralExpression(ast::LiteralExpression* expr) override {
    if (expr->literal_.type == lex::TokenType::IDENTIFIER) {
      ast::Symbol* symbol = expr->scope->Lookup(expr->literal_.GetIdentifier(),
                                                expr->literal_.location);
      if (symbol == nullptr) {
        throw ast::errors::UndefinedSymbolError(
            expr->literal_.GetIdentifier(), expr->literal_.location.Format());
      }
    }

    BaseVisitor::VisitLiteralExpression(expr);
  }
};
}  // namespace passes