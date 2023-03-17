#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/base_visitor.hpp>
#include <ast/symbol/symbol_error.hpp>

namespace ast {
/// Builds symbol table and checks for definitions
class DefinitionChecker : public BaseVisitor {
 public:
  void VisitLiteralExpression(LiteralExpression* expr) override {
        if (expr->literal_.type == lex::TokenType::IDENTIFIER) {
          Symbol* symbol =
              expr->scope->Lookup(expr->literal_.GetIdentifier(), expr->literal_.location);
          if (symbol == nullptr) {
            throw error::UndefinedSymbolError(expr->literal_.GetIdentifier(),
                                              expr->literal_.location.Format());
          }
        }

    BaseVisitor::VisitLiteralExpression(expr);
  }
};
}  // namespace ast