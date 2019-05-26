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
   using std::string_view;

   bool is_ascii_escape(string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) == 2 or distance(escape) == 4);
      LINGUA_EXPECTS(escape[0] == '\\');
      LINGUA_EXPECTS(distance(escape) == 4 ? escape[1] == 'x' : true);

      if (distance(escape) == 2) {
         static auto const valid_escapes = std::unordered_set<char>{'n', 'r', 't', '\\', '0'};
         return valid_escapes.find(escape.back()) != end(valid_escapes);
      }

      auto const leading = escape[2];
      return cjdb::isdigit(leading) and leading <= '7' and cjdb::isxdigit(escape.back());
   }

   bool is_byte_escape(string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) == 2 or distance(escape) == 4);
      LINGUA_EXPECTS(escape[0] == '\\');
      LINGUA_EXPECTS(distance(escape) == 4 ? escape[1] == 'x' : true);

      return distance(escape) == 2 ? is_ascii_escape(escape)
                                   : cjdb::isxdigit(escape[2]) and cjdb::isxdigit(escape[3]);
   }

   using namespace std::string_view_literals;
   constexpr auto unicode_escape_prefix = R"(\u{)"sv;
   constexpr auto unicode_escape_suffix = '}';

   bool is_unicode_escape(string_view const escape) noexcept
   {
      LINGUA_EXPECTS(distance(escape) > 4);
      LINGUA_EXPECTS(escape.starts_with(unicode_escape_prefix));
      LINGUA_EXPECTS(escape.ends_with(unicode_escape_suffix));
      LINGUA_EXPECTS(ranges::count(escape, unicode_escape_suffix) == 1);

      using ranges::all_of, ranges::find;
      using ranges::end, ranges::next, ranges::prev;
      namespace view = ranges::view;

      auto not_suffix = [](auto const c) noexcept { return c != unicode_escape_suffix; };
      auto unicode = escape | view::drop(3) | view::take_while(not_suffix);
      return distance(unicode) <= 6 and all_of(unicode, cjdb::isxdigit);
   }
} // namespace lingua
