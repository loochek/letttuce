#pragma once

#include <ast/syntax_tree.hpp>
#include <ast/expressions.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

class Statement : public TreeNode {
 public:
  void Accept(Visitor* /* visitor */) override {};
};

//////////////////////////////////////////////////////////////////////

class ExprStatement : public Statement {
 public:
  explicit ExprStatement(Expression* expr) : expr_{expr} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitExprStatement(this);
  }

  lex::Location GetLocation() override {
    return expr_->GetLocation();
  }

  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////

class AssignmentStatement : public Statement {
 public:
  explicit AssignmentStatement(LvalueExpression* lhs, Expression* rhs) : lhs_{lhs}, rhs_{rhs} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitAssignmentStatement(this);
  }

  lex::Location GetLocation() override {
    return lhs_->GetLocation();
  }

  LvalueExpression* lhs_;
  Expression* rhs_;
};

//////////////////////////////////////////////////////////////////////
