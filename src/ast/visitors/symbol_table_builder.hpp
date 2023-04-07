#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/base_visitor.hpp>
#include <ast/symbol/symbol_error.hpp>

namespace ast {
class SymbolTableBuilder : public BaseVisitor {
 public:
  void VisitProgram(Program* prg) override {
    // Root scope
    PushScope(lex::Location{});
    // Despite convention below, set program scope as root scope
    // to give access to the root scope
    prg->scope = current_scope_;
    BaseVisitor::VisitProgram(prg);
    PopScope();
  }

  void VisitBlockExpression(BlockExpression* expr) override {
    expr->scope = current_scope_;
    PushScope(expr->GetLocation());
    BaseVisitor::VisitBlockExpression(expr);
    PopScope();
  }

  void VisitVarDeclaration(VarDeclStatement* decl) override {
    decl->scope = current_scope_;
    BaseVisitor::VisitVarDeclaration(decl);

    bool success = current_scope_->AddSymbol(Symbol{.type = SymbolType::VarDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .global_scope = global_scope_,
                                        .symbol = VarSymbol{ .type = decl->type_ }});
    if (!success) {
      throw error::RedefinitionError(decl->GetName(),
                                     decl->GetLocation().Format());
    }
  }

  void VisitFunDeclaration(FunDeclStatement* decl) override {
    decl->scope = current_scope_;
    bool success = current_scope_->AddSymbol(Symbol{.type = SymbolType::FnDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .global_scope = global_scope_,
                                        .symbol = FnSymbol{ .type = decl->type_ }});
    if (!success) {
      throw error::RedefinitionError(decl->GetName(),
                                     decl->GetLocation().Format());
    }

    auto func_type = dynamic_cast<types::FunctionType*>(decl->type_);
    FMT_ASSERT(func_type != nullptr, "FunDeclStatement incorrect type");
    auto& param_types = func_type->GetArgTypes();

    PushScope(decl->GetLocation());
    global_scope_ = false;

    for (size_t i = 0; i < decl->params_.size(); i++) {
      lex::Token& param_token = decl->params_[i];
      types::Type* param_type = param_types[i];
      current_scope_->AddSymbol(Symbol{.type = SymbolType::VarDecl,
                                      .name = param_token.GetIdentifier(),
                                      .location = param_token.location,
                                      .global_scope = global_scope_,
                                      .symbol = VarSymbol{ .type =  param_type }});
    }

    BaseVisitor::VisitFunDeclaration(decl);
    global_scope_ = true;
    PopScope();
  }

  void VisitLiteralExpression(LiteralExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitLiteralExpression(expr);
  }

  void VisitComparisonExpression(ComparisonExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitComparisonExpression(expr);
  }

  void VisitBinaryExpression(BinaryExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitBinaryExpression(expr);
  }

  void VisitUnaryExpression(UnaryExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitUnaryExpression(expr);
  }

  void VisitIfExpression(IfExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitIfExpression(expr);
  }

  void VisitFnCallExpression(FnCallExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitFnCallExpression(expr);
  }

  void VisitVarAccessExpression(VarAccessExpression* expr) override {
    // Not used????
    expr->scope = current_scope_;
    BaseVisitor::VisitVarAccessExpression(expr);
  }

  void VisitYieldExpression(YieldExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitYieldExpression(expr);
  }

  void VisitReturnExpression(ReturnExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitReturnExpression(expr);
  }

  void VisitExprStatement(ExprStatement* stmt) override {
    stmt->scope = current_scope_;
    BaseVisitor::VisitExprStatement(stmt);
  }

  void VisitAssignmentStatement(AssignmentStatement* stmt) override {
    stmt->scope = current_scope_;
    BaseVisitor::VisitAssignmentStatement(stmt);
  }

 private:
  void PushScope(lex::Location location) {
    current_scope_ = new Scope{location, current_scope_};
  }

  void PopScope() {
    FMT_ASSERT(current_scope_ != nullptr, "Tried to escape root scope");
    current_scope_ = current_scope_->GetParent();
  }

 private:
  Scope* current_scope_ = nullptr;
  bool global_scope_ = true;
};
}  // namespace ast