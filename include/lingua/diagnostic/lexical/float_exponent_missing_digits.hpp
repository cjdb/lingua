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
#ifndef LINGUA_DIAGNOSTIC_LEXICAL_FLOAT_EXPONENT_MISSING_DIGITS_HPP
#define LINGUA_DIAGNOSTIC_LEXICAL_FLOAT_EXPONENT_MISSING_DIGITS_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <cjdb/cctype/isdigit.hpp>
#include <fmt/format.h>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <string>

namespace lingua {
   class float_exponent_missing_digits
   : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit float_exponent_missing_digits(std::u8string_view const float_literal,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, generate_message(float_literal)}
      {
         LINGUA_EXPECTS(not ranges::empty(float_literal));
         LINGUA_EXPECTS(ends_with_exponent(float_literal));
      }
   private:
      static std::u8string generate_message(std::u8string_view const float_literal) noexcept
      { return fmt::format(u8R"(floating-point exponent lacking digits: `{}`)", float_literal); }

      static bool ends_with_exponent(std::u8string_view const float_literal) noexcept
      {
         constexpr auto is_e = [](auto const c) constexpr { return c == 'e' or c == 'E'; };
         using ranges::begin, ranges::end, ranges::next, ranges::prev;
         auto const exponent = ranges::find_if(float_literal, is_e);
         auto const distance = ranges::distance(exponent, end(float_literal));
         return (distance == 1 or
                 (distance == 2 and (*next(exponent) == '+' or *next(exponent) == '-')))
            and ranges::all_of(begin(float_literal), exponent, [](auto const c) {
               return cjdb::isdigit(c) or c == '.'; });
      }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_LEXICAL_FLOAT_EXPONENT_MISSING_DIGITS_HPP
