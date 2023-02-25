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
  ComparisonExpression(lex::Token operation, Expression* lhs, Expression* rhs)
      : operation_{operation}, lhs_{lhs}, rhs_{rhs} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitComparisonExpression(this);
  }

  lex::Location GetLocation() override {
    return operation_.location;
  }

  lex::Token operation_;
  Expression* lhs_;
  Expression* rhs_;
};

//////////////////////////////////////////////////////////////////////

// Binary arithmetic: + - / *

class BinaryExpression : public Expression {
 public:
  BinaryExpression(lex::Token operation, Expression* lhs, Expression* rhs)
      : operation_{operation}, lhs_{lhs}, rhs_{rhs} {
  }

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
  UnaryExpression(lex::Token operation, Expression* expr)
      : operation_{operation}, expr_{expr} {
  }

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
  FnCallExpression(lex::Token fn_name, std::vector<Expression*> args)
      : fn_name_{fn_name}, args_{std::move(args)} {
  }

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
  explicit BlockExpression(std::vector<Statement*> statements,
                           Expression* tail_expr)
      : statements_{std::move(statements)}, tail_expr_(tail_expr) {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitBlockExpression(this);
  }

  lex::Location GetLocation() override {
    // return statements_[0]->GetLocation(); cyclic include :(
    return lex::Location{};
  }

  std::vector<Statement*> statements_;
  Expression* tail_expr_;
};

//////////////////////////////////////////////////////////////////////

class IfExpression : public Expression {
 public:
  IfExpression(lex::Token if_token, Expression* condition,
               Expression* then_branch, Expression* else_branch = nullptr)
      : if_token_{if_token},
        condition_{condition},
        then_branch_{then_branch},
        else_branch_{else_branch} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitIfExpression(this);
  }

  lex::Location GetLocation() override {
    return if_token_.location;
  }

  lex::Token if_token_;
  Expression* condition_;
  Expression* then_branch_;
  Expression* else_branch_;
};

//////////////////////////////////////////////////////////////////////

class LiteralExpression : public Expression {
 public:
  explicit LiteralExpression(lex::Token literal) : literal_{literal} {
  }

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
  explicit VarAccessExpression(lex::Token name) : name_{name} {
  }

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
  explicit ReturnExpression(lex::Token return_token, Expression* expr)
      : return_token_{return_token}, expr_{expr} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitReturnExpression(this);
  }

  lex::Location GetLocation() override {
    return return_token_.location;
  }

  lex::Token return_token_;
  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////

class YieldExpression : public Expression {
 public:
  explicit YieldExpression(lex::Token yield_token, Expression* expr)
      : yield_token_{yield_token}, expr_{expr} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitYieldExpression(this);
  }

  lex::Location GetLocation() override {
    return yield_token_.location;
  }

  lex::Token yield_token_;
  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////
