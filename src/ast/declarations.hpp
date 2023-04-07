#pragma once

#include <ast/statements.hpp>
#include <lex/token.hpp>
#include <types/type.hpp>

#include <vector>

//////////////////////////////////////////////////////////////////////

namespace ast {
class Declaration : public ast::Statement {
 public:
  void Accept(Visitor*) override {};

  virtual std::string_view GetName() = 0;
};

class Program : public ast::TreeNode {
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

class VarDeclStatement : public Declaration {
 public:
  VarDeclStatement(lex::Token name, types::Type* type, Expression* init_expr)
      : name_{name}, type_{type}, init_expr_{init_expr} {
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
  types::Type* type_;
  Expression* init_expr_;
};

class FunDeclStatement : public Declaration {
 public:
  FunDeclStatement(lex::Token name, std::vector<lex::Token> params, types::FunctionType* type, Expression* body)
      : name_{name}, params_{std::move(params)}, type_{type}, body_{body} {
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
  types::FunctionType* type_;
  Expression* body_;
};
}  // namespace ast