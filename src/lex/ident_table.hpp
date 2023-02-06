#pragma once

#include <lex/token_type.hpp>

#include <string>
#include <map>

#include <fmt/core.h>

namespace lex {
    class IdentTable {
     public:
        IdentTable() {
            Populate();
        }

        // Use-of-string-view-for-map-lookup
        // https://stackoverflow.com/questions/35525777

        TokenType LookupWord(const std::string_view word) {
            auto iter = map_.find(word);
            if (iter == map_.end()) {
              // Word is identifier if not some keyword
              return TokenType::IDENTIFIER;
            }

            return iter->second;
        }

     private:
        void Populate() {
            map_["true"] = TokenType::TRUE;
            map_["false"] = TokenType::FALSE;
            map_["fun"] = TokenType::FUN;
            map_["var"] = TokenType::VAR;
            map_["if"] = TokenType::IF;
            map_["then"] = TokenType::THEN;
            map_["else"] = TokenType::ELSE;
            map_["for"] = TokenType::FOR;
            map_["return"] = TokenType::RETURN;
            map_["yield"] = TokenType::YIELD;

            map_["Int"] = TokenType::TY_INT;
            map_["Bool"] = TokenType::TY_BOOL;
            map_["String"] = TokenType::TY_STRING;
            map_["Unit"] = TokenType::TY_UNIT;
        }

     private:
        // What-are-transparent-comparators
        // https://stackoverflow.com/questions/20317413

        std::map<std::string, TokenType, std::less<>> map_;
    };

}  // namespace lex
