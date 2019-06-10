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
#include "lingua/lexer/is_escape.hpp"
#include "lingua/utility/contract.hpp"
#include <cjdb/cctype/isdigit.hpp>
#include <cjdb/cctype/isxdigit.hpp>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/view.hpp>
#include <string_view>
#include <unordered_set>

namespace lingua {
   using ranges::distance;
   using std::u8string_view;

   [[nodiscard]] bool is_ascii_escape(u8string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) == 2 or distance(escape) == 4);
      LINGUA_EXPECTS(escape[0] == u8'\\');
      LINGUA_EXPECTS(distance(escape) == 4 ? escape[1] == u8'x' : true);

      if (distance(escape) == 2) {
         static auto const valid_escapes = std::unordered_set{u8'n', u8'r', u8't', u8'\\', u8'0'};
         return valid_escapes.find(escape.back()) != end(valid_escapes);
      }

      auto const leading = escape[2];
      return isdigit(leading) and leading <= u8'7' and isxdigit(escape.back());
   }

   [[nodiscard]] bool is_byte_escape(u8string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) == 2 or distance(escape) == 4);
      LINGUA_EXPECTS(escape[0] == u8'\\');
      LINGUA_EXPECTS(distance(escape) == 4 ? escape[1] == u8'x' : true);

      return distance(escape) == 2 ? is_ascii_escape(escape)
                                   : isxdigit(escape[2]) and isxdigit(escape[3]);
   }

   using namespace std::string_view_literals;
   constexpr auto unicode_escape_prefix = u8R"(\u{)"sv;
   constexpr auto unicode_escape_suffix = u8'}';
   constexpr auto unicode_escape_distance_lower_bound = 5;

   [[nodiscard]] bool is_unicode_escape(u8string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) >= unicode_escape_distance_lower_bound);
      LINGUA_EXPECTS(escape.starts_with(unicode_escape_prefix));
      LINGUA_EXPECTS(escape.ends_with(unicode_escape_suffix));
      LINGUA_EXPECTS(ranges::count(escape, unicode_escape_suffix) == 1);

      using ranges::all_of, ranges::find;
      using ranges::end, ranges::next, ranges::prev;
      namespace view = ranges::view;

      auto not_suffix = [](auto const c) noexcept { return c != unicode_escape_suffix; };
      auto unicode = escape | view::drop(3) | view::take_while(not_suffix);

      constexpr auto unicode_distance_upper_bound = 7;
      return distance(unicode) < unicode_distance_upper_bound and all_of(unicode, isxdigit);
   }
} // namespace lingua
