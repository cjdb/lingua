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
#ifndef LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNTERMINATED_COMMENT_HPP
#define LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNTERMINATED_COMMENT_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <fmt/format.h>
#include <range/v3/distance.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/adjacent_filter.hpp>
#include <range/v3/view/take_while.hpp>
#include <string_view>

namespace lingua {
   class unterminated_comment : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;

   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit unterminated_comment(std::u8string_view const comment,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, format_diagnostic(comment)}
      {
         using namespace std::string_view_literals;
         LINGUA_EXPECTS(comment.starts_with(u8"/*"));
         LINGUA_EXPECTS(unbalanced_delimiters(comment));
      }

   private:
      static std::u8string format_diagnostic(std::u8string_view const comment) noexcept
      {
         constexpr auto message = u8"unterminated multi-line comment starting with:\n"
                                  u8"\t{}\n"
                                  u8"\t[note: multi-line comments in Rust may nest]";

         using namespace ranges;
         auto const first_line = view::take_while([](auto const c) noexcept { return c != u8'\n'; });
         return fmt::format(message, first_line(comment) | to<std::u8string>);
      }

      static bool unbalanced_delimiters(std::u8string_view const comment) noexcept
      {
         namespace view = ranges::view;
         auto comment_openings = comment
                               | view::adjacent_filter([](auto const x, auto const y) {
                                    return x == '/' and y == '*';
                               });
         auto comment_closings = comment
                               | view::adjacent_filter([](auto const x, auto const y) {
                                    return x == '*' and y == '/';
                               });
         return ranges::distance(comment_openings) > ranges::distance(comment_closings);
      }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_DIAGNOSTIC_LEXICAL_UNTERMINATED_COMMENT_HPP
