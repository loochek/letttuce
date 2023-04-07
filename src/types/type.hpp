#pragma once

#include <string>
#include <vector>
#include <fmt/ranges.h>

namespace types {

class Type {
 public:
  virtual ~Type() = default;

  virtual std::string Format() const = 0;
  virtual bool Equals(types::Type* other) const = 0;
};

class PointerType: public Type {
 public:
  explicit PointerType(types::Type* underlying_type) :
        underlying_type_(underlying_type) {}

  std::string Format() const override {
    return fmt::format("*{}", underlying_type_->Format());
  }

  bool Equals(types::Type* other) const override {
    if (auto other_prim = dynamic_cast<PointerType*>(other)) {
      return other_prim->underlying_type_->Equals(underlying_type_);
    }

    return false;
  }

 private:
  types::Type* underlying_type_;
};

class FunctionType: public Type {
 public:
  FunctionType(types::Type* return_type, std::vector<types::Type*> arg_types) :
        return_type_(return_type), arg_types_(std::move(arg_types)) {}

  std::string Format() const override {
    std::vector<std::string> arg_type_strs;
    for (types::Type* arg_type : arg_types_) {
      arg_type_strs.push_back(arg_type->Format());
    }

    return fmt::format("[{}] -> {}", fmt::join(arg_type_strs, ", "), return_type_->Format());
  }

  bool Equals(types::Type* other) const override {
    auto other_func = dynamic_cast<FunctionType*>(other);
    if (other_func == nullptr) {
      return false;
    }

    if (!return_type_->Equals(other_func->return_type_)) {
      return false;
    }

    if (arg_types_.size() != other_func->arg_types_.size()) {
      return false;
    }

    for (size_t i = 0; i < arg_types_.size(); i++) {
      if (!arg_types_[i]->Equals(other_func->arg_types_[i])) {
        return false;
      }
    }

    return true;
  }

  const auto& GetArgTypes() const {
    return arg_types_;
  }

  auto GetReturnType() const {
    return return_type_;
  }

 private:
  types::Type* return_type_;
  std::vector<types::Type*> arg_types_;
};
}

template <>
struct fmt::formatter<types::Type*> : formatter<std::string> {
  auto format(types::Type* c, format_context& ctx) {
    return formatter<std::string>::format(c->Format(), ctx);
  }
};