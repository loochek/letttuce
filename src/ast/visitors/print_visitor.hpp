#pragma once

#include <fmt/core.h>
#include <ast/visitors/visitor.hpp>
#include <ast/expressions.hpp>
#include <ast/statements.hpp>
#include <ast/declarations.hpp>

namespace ast {
#define INDENTED(stmt) { indent(); stmt; }

class PrintVisitor : public Visitor {
 public:
  template<typename Func>
  void IdentBlock(Func fn) {
    curr_tabs_++;
    fn();
    curr_tabs_--;
  }

  void VisitProgram(ast::Program* prg) override  {
    INDENTED(fmt::print("Program\n"));
    for (size_t i = 0; i < prg->decls_.size(); i++) {
      INDENTED(fmt::print("Declaration {}:\n", i));
      IdentBlock([&]() { prg->decls_[i]->Accept(this); });
    }
  }

  void VisitComparisonExpression(ast::ComparisonExpression* expr) override {
    INDENTED(fmt::print("Comparison: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("LHS:\n"));
    IdentBlock([&]() { expr->lhs_->Accept(this); });
    INDENTED(fmt::print("RHS\n"));
    IdentBlock([&]() { expr->rhs_->Accept(this); });
  }

  void VisitBinaryExpression(ast::BinaryExpression* expr) override {
    INDENTED(fmt::print("Binary expression: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("LHS:\n"));
    IdentBlock([&]() { expr->lhs_->Accept(this); });
    INDENTED(fmt::print("RHS\n"));
    IdentBlock([&]() { expr->rhs_->Accept(this); });
  }

  void VisitUnaryExpression(ast::UnaryExpression* expr) override {
    INDENTED(fmt::print("Unary expression: {}\n", lex::FormatTokenType(expr->operation_.type)));
    INDENTED(fmt::print("Expression:\n"));
    IdentBlock([&]() { expr->expr_->Accept(this); });
  }

  void VisitIfExpression(ast::IfExpression* expr) override {
    INDENTED(fmt::print("If\n"));
    INDENTED(fmt::print("Condition:\n"));
    IdentBlock([&]() { expr->condition_->Accept(this); });
    INDENTED(fmt::print("Then branch:\n"));
    IdentBlock([&]() { expr->then_branch_->Accept(this); });
    if (expr->else_branch_ != nullptr) {
      INDENTED(fmt::print("Else branch:\n"));
      IdentBlock([&]() { expr->else_branch_->Accept(this); });
    }
  }

  void VisitBlockExpression(ast::BlockExpression* expr) override {
    INDENTED(fmt::print("Block expression\n"));
    for (size_t i = 0; i < expr->statements_.size(); i++) {
      INDENTED(fmt::print("Statement {}:\n", i));
      IdentBlock([&]() { expr->statements_[i]->Accept(this); });
    }
  }

  void VisitFnCallExpression(ast::FnCallExpression* expr) override {
    INDENTED(fmt::print("Function call\n"));
    INDENTED(fmt::print("Callable:\n"));
    IdentBlock([&]() { expr->callable_->Accept(this); });
    for (size_t i = 0; i < expr->args_.size(); i++)  {
      INDENTED(fmt::print("Arg {}:\n", i));
      IdentBlock([&]() { expr->args_[i]->Accept(this); });
    }
  }

  void VisitLiteralExpression(ast::LiteralExpression* expr) override {
    INDENTED(fmt::print("Literal expression: {}\n", std::string{expr->literal_}));
  }

  void VisitVarAccessExpression(ast::VarAccessExpression* expr) override {
    INDENTED(fmt::print("Var access: {}\n", expr->name_.GetIdentifier()));
  }

  void VisitReturnExpression(ast::ReturnExpression* expr) override {
    INDENTED(fmt::print("Return\n"));
    INDENTED(fmt::print("Value (expression):\n"));
    IdentBlock([&]() { expr->expr_->Accept(this); });
  }

  void VisitYieldExpression(ast::YieldExpression* expr) override {
    INDENTED(fmt::print("Yield\n"));
    INDENTED(fmt::print("Value (expression):\n"));
    IdentBlock([&]() { expr->expr_->Accept(this); });
  }

  void VisitExprStatement(ast::ExprStatement* stmt) override {
    INDENTED(fmt::print("Expression statement\n"));
    IdentBlock([&]() { stmt->expr_->Accept(this); });
  }

  void VisitAssignmentStatement(ast::AssignmentStatement* stmt) override {
    INDENTED(fmt::print("Assignment\n"));
    INDENTED(fmt::print("Lvalue:\n"));
    IdentBlock([&]() { stmt->lhs_->Accept(this); });
    INDENTED(fmt::print("Assigned expression:\n"));
    IdentBlock([&]() { stmt->rhs_->Accept(this); });
  }

  void VisitVarDeclaration(ast::VarDeclStatement* decl) override {
    INDENTED(fmt::print("Variable declaration: {} of type {}\n", decl->GetName(), decl->type_->Format()));
    INDENTED(fmt::print("Initializer:\n"));
    IdentBlock([&]() { decl->init_expr_->Accept(this); });
  }

  void VisitFunDeclaration(ast::FunDeclStatement* decl) override {
    INDENTED(fmt::print("Function declaration: {} of type {}\n", decl->GetName(), decl->type_->Format()));

    INDENTED(fmt::print("Params: "));
    for (auto& param : decl->params_) {
      fmt::print("{}", param.GetIdentifier());
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
}