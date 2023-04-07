#pragma once

#include <ast/syntax_tree.hpp>
#include <ast/expressions.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

namespace ast {

class Statement : public ast::TreeNode {
 public:
  void Accept(Visitor* /* visitor */) override{};
};

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

class AssignmentStatement : public Statement {
 public:
  explicit AssignmentStatement(lex::Token assn_token, LvalueExpression* lhs,
                               Expression* rhs)
      : assn_token_{assn_token}, lhs_{lhs}, rhs_{rhs} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitAssignmentStatement(this);
  }

  lex::Location GetLocation() override {
    return assn_token_.location;
  }

  lex::Token assn_token_;
  LvalueExpression* lhs_;
  Expression* rhs_;
};
}  // namespace ast
