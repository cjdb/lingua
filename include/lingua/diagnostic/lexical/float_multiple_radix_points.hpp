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
#ifndef LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_FLOAT_MULTIPLE_RADIX_POINTS_HPP
#define LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_FLOAT_MULTIPLE_RADIX_POINTS_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <cjdb/cctype/isdigit.hpp>
#include <fmt/format.h>
#include <range/v3/algorithm/adjacent_find.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/size.hpp>
#include <string>
#include <string_view>

namespace lingua {
   class float_multiple_radix_points : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;

   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit float_multiple_radix_points(std::u8string_view const literal,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, generate_help_message(literal)}
      {
         LINGUA_EXPECTS(ranges::count(literal, u8'.') > 1);
         LINGUA_EXPECTS(ranges::adjacent_find(literal, [](auto const x, auto const y) {
            return (x == y) and (x == u8'.');
         }) == ranges::end(literal));
         LINGUA_EXPECTS(ranges::any_of(literal, cjdb::isdigit));
      }

   private:
      static std::u8string generate_help_message(std::u8string_view const literal) noexcept
      {
         constexpr auto help_message_template =
            u8"floating-point literal `{}` has {} radix-points: it must have at most one.";
         return fmt::format(help_message_template, literal, ranges::count(literal, u8'.'));
      }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_FLOAT_MULTIPLE_RADIX_POINTS_HPP
