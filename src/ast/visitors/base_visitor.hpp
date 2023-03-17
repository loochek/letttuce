#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/visitor.hpp>

namespace ast {

/// Base traversal behavior, so you can redefine only necessary parts
class BaseVisitor : public Visitor {
 public:
  void VisitProgram(Program* prg) override {
    for (Declaration* decl : prg->decls_) {
      decl->Accept(this);
    }
  }

  void VisitComparisonExpression(ComparisonExpression* expr) override {
    expr->lhs_->Accept(this);
    expr->rhs_->Accept(this);
  }

  void VisitBinaryExpression(BinaryExpression* expr) override {
    expr->lhs_->Accept(this);
    expr->rhs_->Accept(this);
  }

  void VisitUnaryExpression(UnaryExpression* expr) override {
    expr->expr_->Accept(this);
  }

  void VisitIfExpression(IfExpression* expr) override {
    expr->condition_->Accept(this);
    expr->then_branch_->Accept(this);
    if (expr->else_branch_ != nullptr) {
      expr->else_branch_->Accept(this);
    }
  }

  void VisitBlockExpression(BlockExpression* expr) override {
    for (Statement* stmt : expr->statements_) {
      stmt->Accept(this);
    }
  }

  void VisitFnCallExpression(FnCallExpression* expr) override {
    expr->callable_->Accept(this);
    for (Expression* arg : expr->args_)  {
      arg->Accept(this);
    }
  }

  void VisitLiteralExpression(LiteralExpression*) override {
    // Leaf node
  }

  void VisitVarAccessExpression(VarAccessExpression*) override {
    // Leaf node
  }

  void VisitYieldExpression(YieldExpression* expr) override {
    expr->expr_->Accept(this);
  }

  void VisitReturnExpression(ReturnExpression* expr) override {
    expr->expr_->Accept(this);
  }

  void VisitExprStatement(ExprStatement* stmt) override {
    stmt->expr_->Accept(this);
  }

  void VisitAssignmentStatement(AssignmentStatement* stmt) override {
    stmt->lhs_->Accept(this);
    stmt->rhs_->Accept(this);
  }

  void VisitVarDeclaration(VarDeclStatement* decl) override {
    decl->expr_->Accept(this);
  }

  void VisitFunDeclaration(FunDeclStatement* decl) override {
    decl->body_->Accept(this);
  }
};
} // namespace ast