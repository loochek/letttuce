#pragma once

#include <fmt/core.h>
#include <error/compile_error.hpp>

namespace types::error {

struct TypeError : ::error::CompileError {};

struct ArithmTypeError : TypeError {
  explicit ArithmTypeError(const std::string& location) {
    message = fmt::format("Arithmetic operation has non-int operand at location {}",
                          location);
  }
};

struct IfConditionTypeError : TypeError {
  explicit IfConditionTypeError(const std::string& location) {
    message = fmt::format("If condition at location {} has non-bool type",
                          location);
  }
};

struct IfBranchesTypeError : TypeError {
  explicit IfBranchesTypeError(const std::string& location) {
    message = fmt::format("If branches at location {} have different types",
                          location);
  }
};

struct FnCallNonFuncTypeError : TypeError {
  explicit FnCallNonFuncTypeError(const std::string& location) {
    message = fmt::format("Function call with non-functional callable at location {}",
                          location);
  }
};

struct FnCallArgCountMismatchError : TypeError {
  explicit FnCallArgCountMismatchError(const std::string& location) {
    message = fmt::format("Wrong amount of arguments for function call at location {}",
                          location);
  }
};

struct FnCallArgTypeMismatchError : TypeError {
  explicit FnCallArgTypeMismatchError(const std::string& location) {
    message = fmt::format("Wrong type of argument for function call at location {}",
                          location);
  }
};

struct VarDeclInitTypeMismatchError : TypeError {
  explicit VarDeclInitTypeMismatchError(const std::string& location) {
    message = fmt::format("Wrong type of initial value during variable declaration at location {}",
                          location);
  }
};

struct BadAssignmentError : TypeError {
  explicit BadAssignmentError(const std::string& location) {
    message = fmt::format("Bad assignment LHS at location {}",
                          location);
  }
};

struct NonVarAssignError : TypeError {
  explicit NonVarAssignError(const std::string& location) {
    message = fmt::format("Bad assignment at location {}: can assign only variables",
                          location);
  }
};

struct AssignmentTypeMismatchError : TypeError {
  explicit AssignmentTypeMismatchError(const std::string& location) {
      message = fmt::format("Different types on the sides of assignment at location {}",
                          location);
  }
};

// TODO: move this check to some other pass?
struct ReturnOutsideFnError : TypeError {
  explicit ReturnOutsideFnError(const std::string& location) {
      message = fmt::format("Return expression outside function at location {}",
                            location);
  }
};

struct WrongReturnTypeError : TypeError {
  explicit WrongReturnTypeError(const std::string& location) {
      message = fmt::format("Wrong type of return expression at location {}",
                            location);
  }
};

}  // namespace types::error
