#pragma once

#include "ast/declarations.hpp"
#include "base_visitor.hpp"
#include "ast/symbol/symbol_error.hpp"

namespace ast {
/// Symbol lookup table builder
class ScopeGenerator : public BaseVisitor {
 public:
  void VisitProgram(Program* prg) override {
    // Root scope
    PushScope(lex::Location{});
    BaseVisitor::VisitProgram(prg);
    PopScope();
  }

  void VisitBlockExpression(BlockExpression* expr) override {
    expr->scope = current_scope;
    PushScope(expr->GetLocation());
    BaseVisitor::VisitBlockExpression(expr);
    PopScope();
  }

  void VisitVarDeclaration(VarDeclStatement* decl) override {
    decl->scope = current_scope;
    bool success =
        current_scope->AddSymbol(Symbol{.type = SymbolType::VarDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .symbol = VarSymbol{}});
    if (!success) {
      throw error::RedefinitionError(decl->GetName(),
                                     decl->GetLocation().Format());
    }

    BaseVisitor::VisitVarDeclaration(decl);
  }

  void VisitFunDeclaration(FunDeclStatement* decl) override {
    decl->scope = current_scope;
    bool success =
        current_scope->AddSymbol(Symbol{.type = SymbolType::FnDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .symbol = FnSymbol{}});
    if (!success) {
      throw error::RedefinitionError(decl->GetName(),
                                     decl->GetLocation().Format());
    }

    PushScope(decl->GetLocation());
    for (lex::Token& param : decl->params_) {
      current_scope->AddSymbol(Symbol{.type = SymbolType::VarDecl,
                                      .name = param.GetIdentifier(),
                                      .location = param.location,
                                      .symbol = VarSymbol{}});
    }

    BaseVisitor::VisitFunDeclaration(decl);
    PopScope();
  }

  void VisitLiteralExpression(LiteralExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitLiteralExpression(expr);
  }

  void VisitComparisonExpression(ComparisonExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitComparisonExpression(expr);
  }

  void VisitBinaryExpression(BinaryExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitBinaryExpression(expr);
  }

  void VisitUnaryExpression(UnaryExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitUnaryExpression(expr);
  }

  void VisitIfExpression(IfExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitIfExpression(expr);
  }

  void VisitFnCallExpression(FnCallExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitFnCallExpression(expr);
  }

  void VisitVarAccessExpression(VarAccessExpression* expr) override {
    // Not used????
    expr->scope = current_scope;
    BaseVisitor::VisitVarAccessExpression(expr);
  }

  void VisitYieldExpression(YieldExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitYieldExpression(expr);
  }

  void VisitReturnExpression(ReturnExpression* expr) override {
    expr->scope = current_scope;
    BaseVisitor::VisitReturnExpression(expr);
  }

  void VisitExprStatement(ExprStatement* stmt) override {
    stmt->scope = current_scope;
    BaseVisitor::VisitExprStatement(stmt);
  }

  void VisitAssignmentStatement(AssignmentStatement* stmt) override {
    stmt->scope = current_scope;
    BaseVisitor::VisitAssignmentStatement(stmt);
  }

 private:
  void PushScope(lex::Location location) {
    current_scope = new Scope{location, current_scope};
  }

  void PopScope() {
    FMT_ASSERT(current_scope != nullptr, "Tried to escape root scope");
    current_scope = current_scope->GetParent();
  }

 private:
  Scope* current_scope = nullptr;
};
}  // namespace ast