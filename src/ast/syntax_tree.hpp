#pragma once

#include <ast/visitors/visitor.hpp>
#include <lex/location.hpp>
#include <ast/symbol/scope.hpp>

//////////////////////////////////////////////////////////////////////

namespace ast {

class TreeNode {
 public:
  virtual ~TreeNode() = default;

  virtual void Accept(Visitor* visitor) = 0;
  virtual lex::Location GetLocation() = 0;

  template <typename T>
  T* as() {
    return dynamic_cast<T*>(this);
  }

 public:
  Scope* scope = nullptr;
};
}  // namespace ast

//////////////////////////////////////////////////////////////////////
