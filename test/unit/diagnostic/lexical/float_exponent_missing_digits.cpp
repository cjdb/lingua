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
#include "lingua/diagnostic/lexical/float_exponent_missing_digits.hpp"

#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>

void check_missing_exponent(std::u8string_view const lexeme) noexcept
{
   using lingua::float_exponent_missing_digits;

   auto const coordinates = lingua_test::make_coordinates(lexeme);
   auto const diagnostic = float_exponent_missing_digits{lexeme, coordinates};
   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_help_message = fmt::format(u8"floating-point exponent lacking digits: `{}`", lexeme);
   CHECK(diagnostic.help_message() == expected_help_message);
}

TEST_CASE("checks the error type for  floating-point numbers with missing exponents") {
   using namespace std::string_view_literals;
   SUBCASE("checks lower-case decimal exponent") {
      check_missing_exponent(u8"0.3e"sv);
      check_missing_exponent(u8"0.3e+"sv);
      check_missing_exponent(u8"0.3e-"sv);
      check_missing_exponent(u8"0.465468543e"sv);
      check_missing_exponent(u8"0465468543e"sv);
      check_missing_exponent(u8"0465468543.e"sv);
   }

   SUBCASE("checks upper-case decimal exponent") {
      check_missing_exponent(u8"0.3E"sv);
      check_missing_exponent(u8"0.3E+"sv);
      check_missing_exponent(u8"0.3E-"sv);
      check_missing_exponent(u8"0.465468543E"sv);
      check_missing_exponent(u8"0465468543E"sv);
      check_missing_exponent(u8"0465468543.E"sv);
   }
}
