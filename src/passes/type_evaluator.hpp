#pragma once

#include <ast/declarations.hpp>
#include <ast/visitors/base_visitor.hpp>
#include <types/type_error.hpp>
#include <types/type.hpp>
#include <types/primitive_types.hpp>

namespace passes {
// Evaluates types of expressions and perform type checking
class TypeEvaluator : public ast::BaseVisitor {
 public:
  void VisitComparisonExpression(ast::ComparisonExpression* expr) override {
    BaseVisitor::VisitComparisonExpression(expr);

    if (!(expr->lhs_->type->Equals(&types::PrimitiveType::int_type) &&
          expr->rhs_->type->Equals(&types::PrimitiveType::int_type))) {
      throw types::errors::ArithmTypeError(expr->GetLocation().Format());
    }

    expr->type = &types::PrimitiveType::bool_type;
  }

  void VisitBinaryExpression(ast::BinaryExpression* expr) override {
    BaseVisitor::VisitBinaryExpression(expr);

    if (!(expr->lhs_->type->Equals(&types::PrimitiveType::int_type) &&
          expr->rhs_->type->Equals(&types::PrimitiveType::int_type))) {
      throw types::errors::ArithmTypeError(expr->GetLocation().Format());
    }

    expr->type = &types::PrimitiveType::int_type;
  }

  void VisitUnaryExpression(ast::UnaryExpression* expr) override {
    BaseVisitor::VisitUnaryExpression(expr);

    if (!expr->expr_->type->Equals(&types::PrimitiveType::int_type)) {
      throw types::errors::ArithmTypeError(expr->GetLocation().Format());
    }

    expr->type = &types::PrimitiveType::int_type;
  }

  void VisitIfExpression(ast::IfExpression* expr) override {
    BaseVisitor::VisitIfExpression(expr);

    if (!expr->condition_->type->Equals(&types::PrimitiveType::bool_type)) {
      throw types::errors::IfConditionTypeError(expr->condition_->GetLocation().Format());
    }

    if (expr->else_branch_ != nullptr && !expr->then_branch_->type->Equals(expr->else_branch_->type)) {
      throw types::errors::IfBranchesTypeError(expr->GetLocation().Format());
    }

    expr->type = expr->then_branch_->type;
  }

  void VisitBlockExpression(ast::BlockExpression* expr) override {
    BaseVisitor::VisitBlockExpression(expr);

    if (expr->statements_.empty()) {
      expr->type = &types::PrimitiveType::unit_type;
    } else if (auto expr_stmt = dynamic_cast<ast::ExprStatement*>(expr->statements_.back())) {
      expr->type = expr_stmt->expr_->type;
    } else {
      expr->type = &types::PrimitiveType::unit_type;
    }
  }

  void VisitFnCallExpression(ast::FnCallExpression* expr) override {
    BaseVisitor::VisitFnCallExpression(expr);

    auto func_type = dynamic_cast<types::FunctionType*>(expr->callable_->type);
    if (func_type == nullptr) {
      throw types::errors::FnCallNonFuncTypeError(expr->GetLocation().Format());
    }

    if (func_type->GetArgTypes().size() != expr->args_.size()) {
      throw types::errors::FnCallArgCountMismatchError(expr->GetLocation().Format());
    }

    for (size_t i = 0; i < expr->args_.size(); i++) {
      if (!func_type->GetArgTypes()[i]->Equals(expr->args_[i]->type)) {
        throw types::errors::FnCallArgTypeMismatchError(expr->GetLocation().Format());
      }
    }

    expr->type = func_type->GetReturnType();
  }

  void VisitLiteralExpression(ast::LiteralExpression* expr) override {
    switch (expr->literal_.type) {
      case lex::TokenType::NUMBER:
        expr->type = &types::PrimitiveType::int_type;
        return;

      case lex::TokenType::STRING:
        expr->type = &types::PrimitiveType::string_type;
        return;

      case lex::TokenType::TRUE:
      case lex::TokenType::FALSE:
        expr->type = &types::PrimitiveType::bool_type;
        return;

      case lex::TokenType::IDENTIFIER: {
        ast::Symbol* symbol = expr->scope->Lookup(expr->literal_.GetIdentifier(), expr->GetLocation());
        FMT_ASSERT(symbol != nullptr, "Unknown symbol at type evaluation stage");

        switch (symbol->type) {
          case ast::SymbolType::VarDecl:
            expr->type = std::get<ast::VarSymbol>(symbol->symbol).type;
            return;

          case ast::SymbolType::FnDecl:
            expr->type = std::get<ast::FnSymbol>(symbol->symbol).type;
            return;

          default:
            FMT_ASSERT(false, "Unknown symbol type");
        }
      }

      default:
        FMT_ASSERT(false, "Unknown literal type");
    }
  }

  void VisitVarAccessExpression(ast::VarAccessExpression* expression) override {
    BaseVisitor::VisitVarAccessExpression(expression);
    // ???
    // Seems to be unused by me :)
  }

  void VisitYieldExpression(ast::YieldExpression* expr) override {
    BaseVisitor::VisitYieldExpression(expr);
    // ???
    // Seems to be unused by me :)
  }

  void VisitReturnExpression(ast::ReturnExpression* expr) override {
    BaseVisitor::VisitReturnExpression(expr);
    if (curr_func_type_ == nullptr) {
      throw types::errors::ReturnOutsideFnError(expr->GetLocation().Format());
    }


    if (!curr_func_type_->GetReturnType()->Equals(expr->expr_->type)) {
      throw types::errors::WrongReturnTypeError(expr->GetLocation().Format());
    }

    expr->type = expr->expr_->type;
  }

  void VisitExprStatement(ast::ExprStatement* stmt) override {
    BaseVisitor::VisitExprStatement(stmt);
  }

  void VisitAssignmentStatement(ast::AssignmentStatement* stmt) override {
    BaseVisitor::VisitAssignmentStatement(stmt);
    auto lhs_lit = dynamic_cast<ast::LiteralExpression*>(stmt->lhs_);
    if (lhs_lit == nullptr || lhs_lit->literal_.type != lex::TokenType::IDENTIFIER) {
      throw types::errors::BadAssignmentError(stmt->GetLocation().Format());
    }

    ast::Symbol* lhs_symbol = lhs_lit->scope->Lookup(lhs_lit->literal_.GetIdentifier(), lhs_lit->GetLocation());
    if (lhs_symbol->type != ast::SymbolType::VarDecl) {
      throw types::errors::NonVarAssignError(stmt->GetLocation().Format());
    }

    if (!stmt->rhs_->type->Equals(std::get<ast::VarSymbol>(lhs_symbol->symbol).type)) {
      throw types::errors::AssignmentTypeMismatchError(stmt->GetLocation().Format());
    }
  }

  void VisitVarDeclaration(ast::VarDeclStatement* decl) override {
    BaseVisitor::VisitVarDeclaration(decl);
    if (!decl->type_->Equals(decl->init_expr_->type)) {
      throw types::errors::VarDeclInitTypeMismatchError(decl->GetLocation().Format());
    }
  }

  void VisitFunDeclaration(ast::FunDeclStatement* decl) override {
    types::FunctionType* old = curr_func_type_;
    curr_func_type_ = decl->type_;
    BaseVisitor::VisitFunDeclaration(decl);
    curr_func_type_ = old;
  }

 private:
  types::FunctionType* curr_func_type_ = nullptr;
};
}  // namespace ast