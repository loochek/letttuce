#pragma once

#include <fmt/core.h>
#include <ast/visitors/visitor.hpp>
#include <ast/expressions.hpp>

#include <sstream>
#include <ast/statements.hpp>
#include <ast/expressions.hpp>
#include <ast/declarations.hpp>

namespace ast {

#define INDENTED(stmt) { indent(); stmt; }

class SerializeVisitor : public Visitor {
 public:
  template <typename Func>
  void IdentBlock(Func fn) {
    curr_tabs_++;
    fn();
    curr_tabs_--;
  }

  void VisitProgram(ast::Program* prg) override {
    INDENTED(out_ << fmt::format("Program\n"));
    for (size_t i = 0; i < prg->decls_.size(); i++) {
      INDENTED(out_ << fmt::format("Declaration {}:\n", i));
      IdentBlock([&]() {
        prg->decls_[i]->Accept(this);
      });
    }
  }

  void VisitComparisonExpression(ast::ComparisonExpression* expr) override {
    INDENTED(out_ << fmt::format("Comparison: {}\n",
                                 lex::FormatTokenType(expr->operation_.type)));
    INDENTED(out_ << fmt::format("LHS:\n"));
    IdentBlock([&]() {
      expr->lhs_->Accept(this);
    });
    INDENTED(out_ << fmt::format("RHS\n"));
    IdentBlock([&]() {
      expr->rhs_->Accept(this);
    });
  }

  void VisitBinaryExpression(ast::BinaryExpression* expr) override {
    INDENTED(out_ << fmt::format("Binary expression: {}\n",
                                 lex::FormatTokenType(expr->operation_.type)));
    INDENTED(out_ << fmt::format("LHS:\n"));
    IdentBlock([&]() {
      expr->lhs_->Accept(this);
    });
    INDENTED(out_ << fmt::format("RHS\n"));
    IdentBlock([&]() {
      expr->rhs_->Accept(this);
    });
  }

  void VisitUnaryExpression(ast::UnaryExpression* expr) override {
    INDENTED(out_ << fmt::format("Unary expression: {}\n",
                                 lex::FormatTokenType(expr->operation_.type)));
    INDENTED(out_ << fmt::format("Expression:\n"));
    IdentBlock([&]() {
      expr->expr_->Accept(this);
    });
  }

  void VisitIfExpression(ast::IfExpression* expr) override {
    INDENTED(out_ << fmt::format("If\n"));
    INDENTED(out_ << fmt::format("Condition:\n"));
    IdentBlock([&]() {
      expr->condition_->Accept(this);
    });
    INDENTED(out_ << fmt::format("Then branch:\n"));
    IdentBlock([&]() {
      expr->then_branch_->Accept(this);
    });
    if (expr->else_branch_ != nullptr) {
      INDENTED(out_ << fmt::format("Else branch:\n"));
      IdentBlock([&]() {
        expr->else_branch_->Accept(this);
      });
    }
  }

  void VisitBlockExpression(ast::BlockExpression* expr) override {
    INDENTED(out_ << fmt::format("Block expression\n"));
    for (size_t i = 0; i < expr->statements_.size(); i++) {
      INDENTED(out_ << fmt::format("Statement {}:\n", i));
      IdentBlock([&]() {
        expr->statements_[i]->Accept(this);
      });
    }
  }

  void VisitFnCallExpression(ast::FnCallExpression* expr) override {
    INDENTED(out_ << fmt::format("Function call\n"));
    INDENTED(out_ << fmt::format("Callable:\n"));
    IdentBlock([&]() {
      expr->callable_->Accept(this);
    });
    for (size_t i = 0; i < expr->args_.size(); i++) {
      INDENTED(out_ << fmt::format("Arg {}:\n", i));
      IdentBlock([&]() {
        expr->args_[i]->Accept(this);
      });
    }
  }

  void VisitLiteralExpression(ast::LiteralExpression* expr) override {
    INDENTED(out_ << fmt::format("Literal expression: {}\n",
                                 std::string{expr->literal_}));
  }

  void VisitVarAccessExpression(ast::VarAccessExpression* expr) override {
    INDENTED(
        out_ << fmt::format("Var access: {}\n", expr->name_.GetIdentifier()));
  }

  void VisitReturnExpression(ast::ReturnExpression* expr) override {
    INDENTED(out_ << fmt::format("Return\n"));
    INDENTED(out_ << fmt::format("Value (expression):\n"));
    IdentBlock([&]() {
      expr->expr_->Accept(this);
    });
  }

  void VisitYieldExpression(ast::YieldExpression* expr) override {
    INDENTED(out_ << fmt::format("Yield\n"));
    INDENTED(out_ << fmt::format("Value (expression):\n"));
    IdentBlock([&]() {
      expr->expr_->Accept(this);
    });
  }

  void VisitExprStatement(ast::ExprStatement* stmt) override {
    INDENTED(out_ << fmt::format("Expression statement\n"));
    IdentBlock([&]() {
      stmt->expr_->Accept(this);
    });
  }

  void VisitAssignmentStatement(ast::AssignmentStatement* stmt) override {
    INDENTED(out_ << fmt::format("Assignment\n"));
    INDENTED(out_ << fmt::format("Lvalue:\n"));
    IdentBlock([&]() {
      stmt->lhs_->Accept(this);
    });
    INDENTED(out_ << fmt::format("Assigned expression:\n"));
    IdentBlock([&]() {
      stmt->rhs_->Accept(this);
    });
  }

  void VisitVarDeclaration(ast::VarDeclStatement* decl) override {
    INDENTED(
        out_ << fmt::format("Variable declaration: {}\n", decl->GetName()));
    INDENTED(out_ << fmt::format("Initializer:\n"));
    IdentBlock([&]() {
      decl->expr_->Accept(this);
    });
  }

  void VisitFunDeclaration(ast::FunDeclStatement* decl) override {
    INDENTED(
        out_ << fmt::format("Function declaration: {}\n", decl->GetName()));

    INDENTED(out_ << fmt::format("Params: "));
    for (auto& param : decl->params_) {
      out_ << fmt::format("{}", param.GetIdentifier());
      out_ << fmt::format(" ");
    }
    out_ << fmt::format("\n");

    INDENTED(out_ << fmt::format("Body:\n"));
    IdentBlock([&]() {
      decl->body_->Accept(this);
    });
  }

  std::string GetSerializedString() const {
    return out_.str();
  }

 private:
  void indent() {
    for (size_t i = 0; i < curr_tabs_; i++) {
      out_ << fmt::format("\t");
    }
  }

 private:
  std::stringstream out_;
  size_t curr_tabs_ = 0;
};
} // namespace ast