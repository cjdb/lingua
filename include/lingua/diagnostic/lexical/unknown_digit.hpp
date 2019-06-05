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
#ifndef LINGUA_DIAGNOSTIC_LEXICAL_UNKNOWN_DIGIT_HPP
#define LINGUA_DIAGNOSTIC_LEXICAL_UNKNOWN_DIGIT_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include "lingua/utility/always_false.hpp"
#include <fmt/format.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/distance.hpp>
#include <string>
#include <string_view>

namespace lingua::detail_unknown_digit {
   enum class kind { binary, octal };

   template<kind k>
   class unknown_digit_impl
   : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
      using u8string_view = std::u8string_view;

   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit unknown_digit_impl(u8string_view const literal, u8string_view::iterator const digit,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, generate_message(literal, digit)}
      {
         LINGUA_EXPECTS(has_correct_prefix(literal));
         LINGUA_EXPECTS(has_invalid_digit(*digit));
      }
   private:
      static constexpr bool has_correct_prefix(u8string_view const literal) noexcept
      {
         if constexpr (k == kind::binary) {
            return literal.starts_with(u8"0b");
         }
         else if constexpr (k == kind::octal) {
            return literal.starts_with(u8"0o");
         }
      }

      static constexpr bool has_invalid_digit(char8_t const digit) noexcept
      {
         if constexpr (k == kind::binary) {
            return u8'1' < digit and digit <= u8'9';
         }
         else if constexpr (k == kind::octal) {
            return digit == u8'8' or digit == u8'9';
         }
      }

      static std::u8string
      generate_message(u8string_view const literal, u8string_view::iterator const digit) noexcept
      {
         using ranges::distance, ranges::begin;
         using namespace std::string_view_literals;

         constexpr auto message = u8"unknown digit `{}` in {} literal `{}`\n"
                                  u8"                                  {}"sv;
         auto arrow = make_arrow(literal, digit);
         if constexpr (k == kind::binary) {
            return fmt::format(message, *digit, u8"binary", literal, arrow);
         }
         else if constexpr (k == kind::octal) {
            return fmt::format(message, *digit, u8"octal", literal, arrow);
         }
         else {
            static_assert(always_false<>, u8"unhandled representation for an unknown digit");
         }
      }

      static std::u8string
      make_arrow(u8string_view const literal, u8string_view::iterator const digit) noexcept
      {
         using ranges::begin, ranges::distance;
         using size_type = u8string_view::size_type;
         auto const arrow_length = static_cast<size_type>(distance(begin(literal), digit));
         return std::u8string(arrow_length, u8' ') + u8'^';
      }
   };

   using unknown_digit_binary = unknown_digit_impl<kind::binary>;
   using unknown_digit_octal = unknown_digit_impl<kind::octal>;
} // namespace lingua::detail_unknown_digit

namespace lingua {
   using detail_unknown_digit::unknown_digit_binary;
   using detail_unknown_digit::unknown_digit_octal;
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_LEXICAL_UNKNOWN_DIGIT_HPP
