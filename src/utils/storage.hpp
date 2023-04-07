#pragma once

#include <vector>
#include <memory>

namespace utils {
// Lazy storage that would clean up everything at the end
template<typename BaseType>
class Storage {
 public:
  template <typename Type, typename... Args>
  Type* CreateType(Args&&... args) {
    auto type = std::make_unique<Type>(std::forward<Args>(args)...);
    Type* raw_ptr = type.get();
    types_.emplace_back(std::move(type));
    return raw_ptr;
  }

 private:
  std::vector<std::unique_ptr<BaseType>> types_;
};
}