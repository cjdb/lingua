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
#include "lingua/diagnostic/lexical/unknown_escape.hpp"

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <functional>
#include <range/v3/algorithm/adjacent_find.hpp>
#include <range/v3/distance.hpp>
#include <string_view>

TEST_CASE("checks unknown ASCII escapes") {
   using lingua::unknown_escape_ascii;
   using namespace std::string_view_literals;

   constexpr auto ill_formed_string = u8R"(this\nwas\ra\mista\5e\xef)"sv;
   constexpr auto find_unknown_escape = [](auto const escape) constexpr noexcept {
      return [escape](auto const x, auto const y) constexpr noexcept {
         return x == u8'\\' and y == escape;
      };
   };

   constexpr auto coordinates = lingua_test::make_coordinates(ill_formed_string);

   SUBCASE("invalid letter") {
      auto const first = ranges::adjacent_find(ill_formed_string, find_unknown_escape(u8'm'));
      auto const diagnostic = unknown_escape_ascii{
         ill_formed_string,
         {first, ranges::next(first, 2)},
         coordinates
      };

      CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
      CHECK(diagnostic.coordinates() == coordinates);

      constexpr auto expected_help_message =
u8R"(unrecognised ASCII escape '\m' in string literal `this\nwas\ra\mista\5e\xef`
                                                              ^~)"sv;
      CHECK(diagnostic.help_message() == expected_help_message);
   }

   SUBCASE("invalid digit") {
      auto const first = ranges::adjacent_find(ill_formed_string, find_unknown_escape(u8'5'));
      auto const diagnostic = unknown_escape_ascii{
         ill_formed_string,
         {first, ranges::next(first, 2)},
         coordinates
      };

      CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
      CHECK(diagnostic.coordinates() == coordinates);

      constexpr auto expected_help_message =
u8R"(unrecognised ASCII escape '\5' in string literal `this\nwas\ra\mista\5e\xef`
                                                                    ^~)"sv;
      CHECK(diagnostic.help_message() == expected_help_message);
   }

   SUBCASE("invalid ASCII code") {
      auto const first = ranges::adjacent_find(ill_formed_string, find_unknown_escape(u8'x'));
      auto const diagnostic = unknown_escape_ascii{
         ill_formed_string,
         {first, ranges::next(first, 4)},
         coordinates
      };

      CHECK(diagnostic.coordinates() == coordinates);

      constexpr auto expected_help_message =
u8R"(unrecognised ASCII escape '\xef' in string literal `this\nwas\ra\mista\5e\xef`
                                                                         ^~~~)"sv;
      CHECK(diagnostic.help_message() == expected_help_message);
   }
}

TEST_CASE("checks unknown byte escapes") {
   using lingua::unknown_escape_byte;
   using namespace std::string_view_literals;

   constexpr auto ill_formed_string = u8R"(this\nwas\ra\mista\5e\xef)"sv;
   constexpr auto find_unknown_escape = [](auto const escape) constexpr noexcept {
      return [escape](auto const x, auto const y) constexpr noexcept {
         return x == u8'\\' and y == escape;
      };
   };

   constexpr auto coordinates = lingua_test::make_coordinates(ill_formed_string);

   SUBCASE("invalid letter") {
      auto const first = ranges::adjacent_find(ill_formed_string, find_unknown_escape(u8'm'));
      auto const diagnostic = unknown_escape_byte{
         ill_formed_string,
         {first, ranges::next(first, 2)},
         coordinates
      };

      CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
      CHECK(diagnostic.coordinates() == coordinates);

      constexpr auto expected_help_message =
u8R"(unrecognised byte escape '\m' in string literal `this\nwas\ra\mista\5e\xef`
                                                             ^~)"sv;
      CHECK(diagnostic.help_message() == expected_help_message);
   }

   SUBCASE("invalid digit") {
      auto const first = ranges::adjacent_find(ill_formed_string, find_unknown_escape(u8'5'));
      auto const diagnostic = unknown_escape_byte{
         ill_formed_string,
         {first, ranges::next(first, 2)},
         coordinates
      };

      CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
      CHECK(diagnostic.coordinates() == coordinates);

      constexpr auto expected_help_message =
u8R"(unrecognised byte escape '\5' in string literal `this\nwas\ra\mista\5e\xef`
                                                                   ^~)"sv;
      CHECK(diagnostic.help_message() == expected_help_message);
   }
}

void check_diagnostic(std::u8string_view const ill_formed_string, std::u8string_view const bad_escape,
   std::u8string_view const expected_help_message) noexcept
{
   auto const coordinates = lingua_test::make_coordinates(ill_formed_string);
   using ranges::begin, ranges::end;
   auto const searcher = std::boyer_moore_horspool_searcher{
      begin(bad_escape),
      end(bad_escape)
   };

   auto const first = std::search(begin(ill_formed_string), end(ill_formed_string), searcher);
   REQUIRE(first != end(ill_formed_string));

   using lingua::unknown_escape_unicode;
   auto const diagnostic = unknown_escape_unicode{
      ill_formed_string,
      {first, ranges::next(first, ranges::distance(bad_escape))},
      coordinates
   };

   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);
   CHECK(diagnostic.help_message() == expected_help_message);
}

TEST_CASE("checks unknown Unicode escapes") {
   using namespace std::string_view_literals;
   constexpr auto ill_formed_string = u8R"(this \u{5tr1ng} \u{69}\u{073} \u{ill}-\u{f0rM3d})"sv;

   SUBCASE("\\u{5tr1ng}") {
      constexpr auto bad_escape = u8"\\u{5tr1ng}"sv;
      constexpr auto expected_help_message =
u8R"(unrecognised Unicode escape '\u{5tr1ng}' in string literal `this \u{5tr1ng} \u{69}\u{073} \u{ill}-\u{f0rM3d}`
                                                                 ^~~~~~~~~~)"sv;
      check_diagnostic(ill_formed_string, bad_escape, expected_help_message);
   }

   SUBCASE("\\u{ill}") {
      constexpr auto bad_escape = u8"\\u{ill}"sv;
      constexpr auto expected_help_message =
u8R"(unrecognised Unicode escape '\u{ill}' in string literal `this \u{5tr1ng} \u{69}\u{073} \u{ill}-\u{f0rM3d}`
                                                                                       ^~~~~~~)"sv;
      check_diagnostic(ill_formed_string, bad_escape, expected_help_message);
   }
   SUBCASE("\\u{f0rM3d}") {
      constexpr auto bad_escape = u8"\\u{f0rM3d}"sv;
      constexpr auto expected_help_message =
u8R"(unrecognised Unicode escape '\u{f0rM3d}' in string literal `this \u{5tr1ng} \u{69}\u{073} \u{ill}-\u{f0rM3d}`
                                                                                                  ^~~~~~~~~~)"sv;
      check_diagnostic(ill_formed_string, bad_escape, expected_help_message);
   }
}
