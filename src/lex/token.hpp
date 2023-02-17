#pragma once

#include <lex/scanner.hpp>

#include <variant>
#include <cstddef>

namespace lex {

//////////////////////////////////////////////////////////////////////

struct Token {
  using DataType = std::variant<std::monostate, int, std::string_view>;

  TokenType type = TokenType::DUMMY;
  Location location;
  DataType data;

  Token() = default;
  Token(TokenType type, Location location,
        DataType data = std::monostate())
      : type{type}, location{location}, data{data} {
  }

  std::string_view GetIdentifier() const {
    FMT_ASSERT(type == TokenType::IDENTIFIER,
               "Requesting the name of non-identifier");
    return std::get<std::string_view>(data);
  }

  explicit operator std::string() {
    if (auto* str = get_if<std::string_view>(&data)) {
      return std::string(*str);
    }

    if (auto* num = get_if<int>(&data)) {
      return std::to_string(*num);
    }

    FMT_ASSERT(false, "Attempt to request a string from token with no data");
  }

  //    std::string Format() const {
  //        return fmt::format("type = {}, data = {}, {}",
  //                           FormatTokenType(type), data, location.Format());
  //    }
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
