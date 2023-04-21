#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/base_visitor.hpp>
#include <ast/symbol/symbol_error.hpp>

namespace passes {
class SymbolTableBuilder : public ast::BaseVisitor {
 public:
  void VisitProgram(ast::Program* prg) override {
    // Root scope
    PushScope(lex::Location{});
    // Despite convention below, set program scope as root scope
    // to give access to the root scope
    prg->scope = current_scope_;
    BaseVisitor::VisitProgram(prg);
    PopScope();
  }

  void VisitBlockExpression(ast::BlockExpression* expr) override {
    expr->scope = current_scope_;
    PushScope(expr->GetLocation());
    BaseVisitor::VisitBlockExpression(expr);
    PopScope();
  }

  void VisitVarDeclaration(ast::VarDeclStatement* decl) override {
    decl->scope = current_scope_;
    BaseVisitor::VisitVarDeclaration(decl);

    bool success = current_scope_->AddSymbol(ast::Symbol{.type = ast::SymbolType::VarDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .global_scope = global_scope_,
                                        .symbol = ast::VarSymbol{ .type = decl->type_ }});
    if (!success) {
      throw ast::errors::RedefinitionError(decl->GetName(),
                                          decl->GetLocation().Format());
    }
  }

  void VisitFunDeclaration(ast::FunDeclStatement* decl) override {
    decl->scope = current_scope_;
    bool success = current_scope_->AddSymbol(ast::Symbol{.type = ast::SymbolType::FnDecl,
                                        .name = decl->GetName(),
                                        .location = decl->GetLocation(),
                                        .global_scope = global_scope_,
                                        .symbol = ast::FnSymbol{ .type = decl->type_ }});
    if (!success) {
      throw ast::errors::RedefinitionError(decl->GetName(),
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
      current_scope_->AddSymbol(ast::Symbol{.type = ast::SymbolType::VarDecl,
                                      .name = param_token.GetIdentifier(),
                                      .location = param_token.location,
                                      .global_scope = global_scope_,
                                      .symbol = ast::VarSymbol{ .type =  param_type }});
    }

    BaseVisitor::VisitFunDeclaration(decl);
    global_scope_ = true;
    PopScope();
  }

  void VisitLiteralExpression(ast::LiteralExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitLiteralExpression(expr);
  }

  void VisitComparisonExpression(ast::ComparisonExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitComparisonExpression(expr);
  }

  void VisitBinaryExpression(ast::BinaryExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitBinaryExpression(expr);
  }

  void VisitUnaryExpression(ast::UnaryExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitUnaryExpression(expr);
  }

  void VisitIfExpression(ast::IfExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitIfExpression(expr);
  }

  void VisitFnCallExpression(ast::FnCallExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitFnCallExpression(expr);
  }

  void VisitVarAccessExpression(ast::VarAccessExpression* expr) override {
    // Not used????
    expr->scope = current_scope_;
    BaseVisitor::VisitVarAccessExpression(expr);
  }

  void VisitYieldExpression(ast::YieldExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitYieldExpression(expr);
  }

  void VisitReturnExpression(ast::ReturnExpression* expr) override {
    expr->scope = current_scope_;
    BaseVisitor::VisitReturnExpression(expr);
  }

  void VisitExprStatement(ast::ExprStatement* stmt) override {
    stmt->scope = current_scope_;
    BaseVisitor::VisitExprStatement(stmt);
  }

  void VisitAssignmentStatement(ast::AssignmentStatement* stmt) override {
    stmt->scope = current_scope_;
    BaseVisitor::VisitAssignmentStatement(stmt);
  }

 private:
  void PushScope(lex::Location location) {
    current_scope_ = new ast::Scope{location, current_scope_};
  }

  void PopScope() {
    FMT_ASSERT(current_scope_ != nullptr, "Tried to escape root scope");
    current_scope_ = current_scope_->GetParent();
  }

 private:
  ast::Scope* current_scope_ = nullptr;
  bool global_scope_ = true;
};
}  // namespace ast