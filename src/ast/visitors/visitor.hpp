#pragma once

//////////////////////////////////////////////////////////////////////

class Expression;
class ComparisonExpression;
class BinaryExpression;
class UnaryExpression;
class DereferenceExpression;
class AddressofExpression;
class IfExpression;
class MatchExpression;
class NewExpression;
class BlockExpression;
class FnCallExpression;
class IntrinsicCall;
class CompoundInitializerExpr;
class FieldAccessExpression;
class LiteralExpression;
class VarAccessExpression;
class TypecastExpression;
class YieldExpression;
class ReturnExpression;

//////////////////////////////////////////////////////////////////////

class Statement;
class ExprStatement;
class AssignmentStatement;
class VarDeclStatement;
class FunDeclStatement;

//////////////////////////////////////////////////////////////////////

class TypeDeclaration;
class ImplDeclaration;
class TraitDeclaration;

//////////////////////////////////////////////////////////////////////

//class BindingPattern;
//class LiteralPattern;
//class StructPattern;
//class VariantPattern;
//class DiscardingPattern;

//////////////////////////////////////////////////////////////////////

class Visitor {
 public:
  virtual ~Visitor() = default;

  virtual void VisitComparisonExpression(ComparisonExpression *expr) = 0;
  virtual void VisitBinaryExpression(BinaryExpression *expr) = 0;
  virtual void VisitUnaryExpression(UnaryExpression *expr) = 0;
//  virtual void VisitDereferenceExpression(DereferenceExpression *expr) = 0;
//  virtual void VisitAddressofExpression(AddressofExpression *expr) = 0;
  virtual void VisitIfExpression(IfExpression *expr) = 0;
//  virtual void VisitMatchExpression(MatchExpression *expr) = 0;
//  virtual void VisitNewExpression(NewExpression *expr) = 0;
  virtual void VisitBlockExpression(BlockExpression *expr) = 0;
  virtual void VisitFnCallExpression(FnCallExpression *expr) = 0;
//  virtual void VisitIntrinsicCall(IntrinsicCall *expr) = 0;
//  virtual void VisitCompoundInitializerExpr(CompoundInitializerExpr *expr) = 0;
//  virtual void VisitFieldAccessExpression(FieldAccessExpression *expr) = 0;
  virtual void VisitLiteralExpression(LiteralExpression *expr) = 0;
  virtual void VisitVarAccessExpression(VarAccessExpression *expr) = 0;
//  virtual void VisitTypecastExpression(TypecastExpression *expr) = 0;
//  virtual void VisitYieldExpression(YieldExpression *expr) = 0;
  virtual void VisitReturnExpression(ReturnExpression *expr) = 0;

  //////////////////////////////////////////////////////////////////////

  virtual void VisitExprStatement(ExprStatement* stmt) = 0;
  virtual void VisitAssignmentStatement(AssignmentStatement* stmt) = 0;

  //////////////////////////////////////////////////////////////////////

  virtual void VisitVarDeclaration(VarDeclStatement* decl) = 0;
  virtual void VisitFunDeclaration(FunDeclStatement* decl) = 0;
//  virtual void TypeDeclaration(TypeDeclaration* decl) = 0;
//  virtual void ImplDeclaration(ImplDeclaration* decl) = 0;
//  virtual void TraitDeclaration(TraitDeclaration* decl) = 0;

  //////////////////////////////////////////////////////////////////////

//  virtual void BindingPattern() = 0;
//  virtual void LiteralPattern() = 0;
//  virtual void StructPattern() = 0;
//  virtual void VariantPattern() = 0;
//  virtual void DiscardingPattern() = 0;
};

//////////////////////////////////////////////////////////////////////
