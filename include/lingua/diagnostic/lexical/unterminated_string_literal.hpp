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
#ifndef LINGUA_DIAGNOSTIC_LEXICAL_UNTERMINATED_STRING_LITERAL_HPP
#define LINGUA_DIAGNOSTIC_LEXICAL_UNTERMINATED_STRING_LITERAL_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/lexer/string_literal_terminated.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <string_view>

namespace lingua {
   class unterminated_string_literal
   : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit unterminated_string_literal(std::u8string_view const literal,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, generate_message(literal)}
      {
         LINGUA_EXPECTS(not lingua::string_literal_terminated(literal));
      }
   private:
      [[nodiscard]] static std::u8string generate_message(std::u8string_view const literal) noexcept
      { return fmt::format(u8"unterminated string literal: `{}`", literal); }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_LEXICAL_UNTERMINATED_STRING_LITERAL_HPP
