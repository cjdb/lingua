//
//  Copyright 2019 Christopher Di Bella
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNKNOWN_TOKEN_HPP
#define LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNKNOWN_TOKEN_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/utility/contract.hpp"
#include "lingua/source_coordinate_range.hpp"
#include <fmt/format.h>
#include <range/v3/algorithm/none_of.hpp>
#include <string_view>

namespace lingua {
   class unknown_token : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit unknown_token(std::u8string_view const lexeme,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, fmt::format(u8R"(unknown token "{}")", lexeme)}
      { LINGUA_EXPECTS(not_valid_token(lexeme)); }
   private:
      static bool not_valid_token(std::u8string_view const lexeme) noexcept
      {
         return ranges::none_of(lexeme, [](auto const c) noexcept {
            return u8' ' < c and c < u8'~' and c != u8'`' and c != u8'\\';
         });
      }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNKNOWN_TOKEN_HPP
