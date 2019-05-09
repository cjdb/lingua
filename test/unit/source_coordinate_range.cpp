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
#include "lingua/source_coordinate_range.hpp"

#include "lingua/source_coordinate.hpp"
#include <doctest.h>

TEST_CASE("checks that source_coordinate_ranges behave correctly")
{
   using lingua::source_coordinate;
   using lingua::source_coordinate_range;

   constexpr auto first1 = source_coordinate{};
   constexpr auto last1 =
      source_coordinate{source_coordinate::line_type{15}, source_coordinate::column_type{7}};
   constexpr auto r1 = source_coordinate_range{first1, last1};

   CHECK(r1.begin() == first1);
   CHECK(r1.end() == last1);

   CHECK(r1 == r1);
   CHECK(not(r1 != r1));

   // check an empty range
   constexpr auto first2 =
      source_coordinate{source_coordinate::line_type{4}, source_coordinate::column_type{18}};
   constexpr auto r2 = source_coordinate_range{first2, first2};
   CHECK(r2.begin() == first2);
   CHECK(r2.end() == first2);
   CHECK(r2.empty());

   // check operator!= is symmetric
   CHECK(r2 != r1);
   CHECK(r1 != r2);

   // check operator== complements operator!=
   CHECK(not(r2 == r1));
   CHECK(not(r1 == r2));

   // check operator== and operator!= are transitive
   constexpr auto r3 = source_coordinate_range{first1, last1};
   REQUIRE(r1 == r3);
   REQUIRE(r3 != r2);

   constexpr auto r4 = r3;
   REQUIRE(r3 == r4);
   CHECK(r1 == r4);
   CHECK(r2 != r4);
}
