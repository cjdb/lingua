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
#include "lingua/diagnostic/lexical/unknown_digit.hpp"

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/iterator/operations.hpp>
#include <string_view>

template<class T>
void check_unknown_digit(std::u8string_view const kind, std::u8string_view const literal,
   std::u8string_view::iterator const digit, std::u8string_view const arrow) noexcept
// [[expects axiom: reachable(literal, digit)]]
{
   auto const coordinates = lingua_test::make_coordinates(literal);
   auto const diagnostic = T{literal, digit, coordinates};

   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_help_message = fmt::format(u8"unknown digit `{}` in {} literal `{}`\n"
                                                  u8"                                  {}",
      *digit, kind, literal, arrow);
   CHECK(expected_help_message == diagnostic.help_message());
}

TEST_CASE("checks the error type for unknown binary digits") {
   using lingua::unknown_digit_binary;
   using namespace std::string_view_literals;

   constexpr auto literal = u8"0b210_1011_0110_6301_4110_1051_0118"sv;
   constexpr auto binary = u8"binary"sv;
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'2'),
      u8"  ^");
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'6'),
      u8"                ^");
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'3'),
      u8"                 ^");
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'4'),
      u8"                     ^");
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'5'),
      u8"                            ^");
   check_unknown_digit<unknown_digit_binary>(binary, literal, ranges::find(literal, u8'8'),
      u8"                                  ^");
}

TEST_CASE("checks the error type for unknown octal digits") {
   using lingua::unknown_digit_octal;
   using namespace std::string_view_literals;

   constexpr auto literal = u8"0o796_950_448"sv;
   constexpr auto octal = u8"octal";
   auto const nine = ranges::find(literal, u8'9');
   check_unknown_digit<unknown_digit_octal>(octal, literal, nine, u8"   ^");
   check_unknown_digit<unknown_digit_octal>(octal, literal, ranges::next(nine, 3), u8"      ^");
   check_unknown_digit<unknown_digit_octal>(octal, literal, ranges::find(literal, u8'8'),
      u8"            ^");
}
