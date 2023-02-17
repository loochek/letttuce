#pragma once

#include <ast/syntax_tree.hpp>

#include <lex/token.hpp>

#include <utility>
#include <vector>

//////////////////////////////////////////////////////////////////////

class Expression : public TreeNode {
 public:
  // Later
  // virtual types::Type* GetType() = 0;
};

//////////////////////////////////////////////////////////////////////

// Assignable entity

class LvalueExpression : public Expression {};

//////////////////////////////////////////////////////////////////////

class ComparisonExpression : public Expression {
 public:
  ComparisonExpression(lex::Token operation, Expression* lhs, Expression* rhs) : operation_{operation}, lhs_{lhs}, rhs_{rhs} {}

  void Accept(Visitor* visitor) override {
    visitor->VisitComparisonExpression(this);
  }

  lex::Location GetLocation() override {
    return lhs_->GetLocation();
  }

  lex::Token operation_;
  Expression* lhs_;
  Expression* rhs_;
};

//////////////////////////////////////////////////////////////////////

// Binary arithmetic: + - / *

class BinaryExpression : public Expression {
 public:
  BinaryExpression(lex::Token operation, Expression* lhs, Expression* rhs) : operation_{operation}, lhs_{lhs}, rhs_{rhs} {}

  void Accept(Visitor* visitor) override {
      visitor->VisitBinaryExpression(this);
  }

  lex::Location GetLocation() override {
      return lhs_->GetLocation();
  }

  lex::Token operation_;
  Expression* lhs_;
  Expression* rhs_;
};

//////////////////////////////////////////////////////////////////////

class UnaryExpression : public Expression {
 public:
  UnaryExpression(lex::Token operation, Expression* expr) : operation_{operation}, expr_{expr} {}

  void Accept(Visitor* visitor) override {
      visitor->VisitUnaryExpression(this);
  }

  lex::Location GetLocation() override {
      return operation_.location;
  }

  lex::Token operation_;
  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////

class FnCallExpression : public Expression {
 public:
  FnCallExpression(lex::Token fn_name, std::vector<Expression*> args) : fn_name_{fn_name}, args_{std::move(args)} {}

  void Accept(Visitor* visitor) override {
      visitor->VisitFnCallExpression(this);
  }

  lex::Location GetLocation() override {
      return fn_name_.location;
  }

  lex::Token fn_name_;
  std::vector<Expression*> args_;
};

//////////////////////////////////////////////////////////////////////

class BlockExpression : public Expression {
 public:
  explicit BlockExpression(std::vector<Statement*> statements): statements_{std::move(statements)} {}

  void Accept(Visitor* visitor) override {
    visitor->VisitBlockExpression(this);
  }

  lex::Location GetLocation() override {
    // return statements_[0]->GetLocation(); cyclic include :(
    return lex::Location{};
  }

  std::vector<Statement*> statements_;
  // TODO: The last must be expression stmt?
};

//////////////////////////////////////////////////////////////////////

class IfExpression : public Expression {
 public:
  IfExpression(Expression *condition, Expression* then_branch, Expression* else_branch = nullptr) :
        condition_{condition}, then_branch_{then_branch}, else_branch_{else_branch} {}

  void Accept(Visitor* visitor) override {
    visitor->VisitIfExpression(this);
  }

  lex::Location GetLocation() override {
    return condition_->GetLocation();
  }

  Expression* condition_;
  Expression* then_branch_;
  Expression* else_branch_;
};

//////////////////////////////////////////////////////////////////////

class LiteralExpression : public Expression {
 public:
  explicit LiteralExpression(lex::Token literal) : literal_{literal} {}

  void Accept(Visitor* visitor) override {
      visitor->VisitLiteralExpression(this);
  }

  lex::Location GetLocation() override {
    return literal_.location;
  }

  lex::Token literal_;
};

//////////////////////////////////////////////////////////////////////

class VarAccessExpression : public LvalueExpression {
 public:
  explicit VarAccessExpression(lex::Token name): name_{name} {}

  void Accept(Visitor* visitor) override {
      visitor->VisitVarAccessExpression(this);
  }

  lex::Location GetLocation() override {
    return name_.location;
  }

  lex::Token name_;
};

//////////////////////////////////////////////////////////////////////

class ReturnExpression : public Expression {
 public:
  explicit ReturnExpression(Expression* expr): expr_{expr} {}

  void Accept(Visitor* visitor) override {
    visitor->VisitReturnExpression(this);
  }

  lex::Location GetLocation() override {
    return expr_->GetLocation();
  }

  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////
