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
#include "lingua/diagnostic/lexical/float_multiple_radix_points.hpp"

#include "lingua/source_coordinate_range.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <fmt/format.h>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/size.hpp>
#include <string_view>

void check_multiple_radix_points(std::u8string_view const literal) noexcept
{
   auto const coordinates = lingua_test::make_coordinates(literal);

   using lingua::float_multiple_radix_points;
   auto const diagnostic = float_multiple_radix_points{literal, coordinates};

   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_help_message = fmt::format(
      u8"floating-point literal `{}` has {} radix-points: it must have at most one.",
      literal, ranges::count(literal, u8'.'));
   CHECK(diagnostic.help_message() == expected_help_message);
}

TEST_CASE("checks the error type for floating-point numbers with multiple radix points") {
   SUBCASE("checks contiguous radix points") {
      SUBCASE("without exponents") {
         check_multiple_radix_points(u8"8198.7.4");
      }
      SUBCASE("checks decimal exponents") {
         check_multiple_radix_points(u8"10.8.4.e");
         check_multiple_radix_points(u8"8.978458.445848.8e65");
      }
   }

   SUBCASE("checks non-contiguous radix points") {
      SUBCASE("without exponents") {
         check_multiple_radix_points(u8"128.0.0.1");
      }
      SUBCASE("checks decimal exponents") {
         check_multiple_radix_points(u8"75632.4325.e456");
         check_multiple_radix_points(u8"75632.4325.E456");
      }
   }
}
