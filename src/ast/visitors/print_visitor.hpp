#pragma once

#include <fmt/core.h>
#include <ast/visitors/visitor.hpp>
#include <ast/expressions.hpp>

#define INDENTED(stmt) { indent(); stmt; }

class PrintVisitor : public Visitor {
 public:
  template<typename Func>
  void IdentBlock(Func fn) {
    curr_tabs_++;
    fn();
    curr_tabs_--;
  }

  void VisitComparisonExpression(ComparisonExpression* expr) override {
    INDENTED(fmt::print("Comparison: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("LHS:\n"));
    IdentBlock([&]() { expr->lhs_->Accept(this); });
    INDENTED(fmt::print("RHS\n"));
    IdentBlock([&]() { expr->rhs_->Accept(this); });
  }

  void VisitBinaryExpression(BinaryExpression* expr) override {
    INDENTED(fmt::print("Binary expression: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("LHS:\n"));
    IdentBlock([&]() { expr->lhs_->Accept(this); });
    INDENTED(fmt::print("RHS\n"));
    IdentBlock([&]() { expr->rhs_->Accept(this); });
  }

  void VisitUnaryExpression(UnaryExpression* expr) override {
    INDENTED(fmt::print("Unary expression: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("Expression:\n"));
    IdentBlock([&]() { expr->expr_->Accept(this); });
  }

  void VisitIfExpression(IfExpression* expr) override {
    INDENTED(fmt::print("If\n"));
    INDENTED(fmt::print("Condition:\n"));
    IdentBlock([&]() { expr->condition_->Accept(this); });
    INDENTED(fmt::print("Then branch:\n"));
    IdentBlock([&]() { expr->then_branch_->Accept(this); });
    INDENTED(fmt::print("Else branch:\n"));
    IdentBlock([&]() { expr->else_branch_->Accept(this); });
  }

  void VisitBlockExpression(BlockExpression* expr) override {
    INDENTED(fmt::print("Block expression\n"));
    for (size_t i = 0; i < expr->statements_.size(); i++) {
      INDENTED(fmt::print("Statement {}:\n", i));
      IdentBlock([&]() { expr->statements_[i]->Accept(this); });
    }
  }

  void VisitFnCallExpression(FnCallExpression* expr) override {
    INDENTED(fmt::print("Function call: \n", expr->fn_name_.GetIdentifier()));
    for (size_t i = 0; i < expr->args_.size(); i++)  {
      INDENTED(fmt::print("Arg {}:\n", i));
      IdentBlock([&]() { expr->args_[i]->Accept(this); });
    }
  }

  void VisitLiteralExpression(LiteralExpression* expr) override {
    INDENTED(fmt::print("Literal expression: {}\n", std::string{expr->literal_}));
  }

  void VisitVarAccessExpression(VarAccessExpression* expr) override {
    INDENTED(fmt::print("Var access: {}\n", expr->name_.GetIdentifier()));
  }

  void VisitReturnExpression(ReturnExpression* expr) override {
    INDENTED(fmt::print("Return\n"));
    INDENTED(fmt::print("Value (expression):\n"));
    IdentBlock([&]() { expr->expr_->Accept(this); });
  }

  void VisitExprStatement(ExprStatement* stmt) override {
    INDENTED(fmt::print("Expression statement\n"));
    IdentBlock([&]() { stmt->expr_->Accept(this); });
  }

  void VisitAssignmentStatement(AssignmentStatement* stmt) override {
    INDENTED(fmt::print("Assignment\n"));
    INDENTED(fmt::print("Lvalue:\n"));
    IdentBlock([&]() { stmt->lhs_->Accept(this); });
    INDENTED(fmt::print("Assigned expression:\n"));
    IdentBlock([&]() { stmt->rhs_->Accept(this); });
  }

  void VisitVarDeclaration(VarDeclStatement* decl) override {
    INDENTED(fmt::print("Variable declaration: {}\n", decl->GetName()));
    INDENTED(fmt::print("Initializer:\n"));
    IdentBlock([&]() { decl->expr_->Accept(this); });
  }

  void VisitFunDeclaration(FunDeclStatement* decl) override {
    INDENTED(fmt::print("Function declaration: {}\n", decl->GetName()));

    INDENTED(fmt::print("Params: "));
    for (auto& param : decl->params_) {
      fmt::print(param.GetIdentifier());
      fmt::print(" ");
    }
    fmt::print("\n");

    INDENTED(fmt::print("Body:\n"));
    IdentBlock([&]() { decl->body_->Accept(this); });
  }

 private:
  void indent() const {
    for (size_t i = 0; i < curr_tabs_; i++) {
      fmt::print("\t");
    }
  }

 private:
  size_t curr_tabs_ = 0;
};