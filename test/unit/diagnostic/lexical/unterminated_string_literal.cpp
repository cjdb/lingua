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
#include "lingua/diagnostic/lexical/unterminated_string_literal.hpp"

#include "lingua/source_coordinate_range.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <fmt/format.h>
#include <range/v3/size.hpp>
#include <string_view>

void check_unterminated_string(std::u8string_view const lexeme) noexcept
{
   using lingua::unterminated_string_literal;

   auto const coordinates = lingua_test::make_coordinates(lexeme);
   auto const diagnostic = unterminated_string_literal{lexeme, coordinates};

   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_message = fmt::format(u8"unterminated string literal: `{}`", lexeme);
   CHECK(diagnostic.help_message() == expected_message);
}

TEST_CASE("checks that floating-point numbers with exponents end with digits") {
   SUBCASE("empty string") {
      check_unterminated_string(u8R"(")");
      check_unterminated_string(u8R"(r")");
      check_unterminated_string(u8R"(r#")");
      check_unterminated_string(u8R"(r#"")");
   }

   SUBCASE("small string") {
      check_unterminated_string(u8R"("hello)");
      check_unterminated_string(u8R"(r"hello)");
      check_unterminated_string(u8R"(r#"hello)");
      check_unterminated_string(u8R"(r#"hello")");
   }
   SUBCASE("large string") {
      check_unterminated_string(u8R"("I wanna be the very best, like no one ever was.)");
      check_unterminated_string(u8R"(r"(I wanna be the very best, like no one ever was.)");
      check_unterminated_string(u8R"(r#"(I wanna be the very best, like no one ever was.)");
      check_unterminated_string(u8R"(r#"(I wanna be the very best, like no one ever was.")");
   }
}
