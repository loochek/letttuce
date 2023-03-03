#pragma once

#include <ast/statements.hpp>

#include <lex/token.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

class Declaration : public Statement {
 public:
  void Accept(Visitor*) override {};

  virtual std::string_view GetName() = 0;
};

//////////////////////////////////////////////////////////////////////

class Program : public TreeNode {
 public:
  void Accept(Visitor* visitor) override {
    visitor->VisitProgram(this);
  }

  explicit Program(std::vector<Declaration*> decls)
      : decls_{std::move(decls)} {
  }

  lex::Location GetLocation() override {
    return lex::Location{};
  }

  std::vector<Declaration*> decls_;
};

//////////////////////////////////////////////////////////////////////

class VarDeclStatement : public Declaration {
 public:
  VarDeclStatement(lex::Token name, Expression* expr)
      : name_{name}, expr_{expr} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitVarDeclaration(this);
  }

  lex::Location GetLocation() override {
    return name_.location;
  }

  std::string_view GetName() override {
    return name_.GetIdentifier();
  }

  lex::Token name_;
  Expression* expr_;
};

//////////////////////////////////////////////////////////////////////

class FunDeclStatement : public Declaration {
 public:
  FunDeclStatement(lex::Token name, std::vector<lex::Token> params, Expression* body)
      : name_{name}, params_{std::move(params)}, body_{body} {
  }

  void Accept(Visitor* visitor) override {
    visitor->VisitFunDeclaration(this);
  }

  std::string_view GetName() override {
    return name_.GetIdentifier();
  }

  lex::Location GetLocation() override {
    return name_.location;
  }

  lex::Token name_;
  std::vector<lex::Token> params_;
  Expression* body_;
};

//////////////////////////////////////////////////////////////////////
