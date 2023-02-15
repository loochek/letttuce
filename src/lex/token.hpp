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
  Token(TokenType type, const Location& location,
        const DataType& data = std::monostate())
      : type(type), location(location), data(data) {
  }

  //    std::string Format() const {
  //        return fmt::format("type = {}, data = {}, {}",
  //                           FormatTokenType(type), data, location.Format());
  //    }
};

//////////////////////////////////////////////////////////////////////

}  // namespace lex
