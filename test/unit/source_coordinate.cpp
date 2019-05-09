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
#include "lingua/source_coordinate.hpp"

#include "cjdb/concepts/core/defaultconstructible.hpp"
#include "cjdb/concepts/core/integral.hpp"
#include "cjdb/concepts/core/same.hpp"
#include "cjdb/concepts/object/regular.hpp"
#include "cjdb/functional/rangecmp/equal_to.hpp"
#include <doctest.h>
#include <fmt/format.h>
#include <type_traits>

TEST_CASE("checks source_coordinate is implemented correctly")
{
   // Tests the source coordinate
   using lingua::source_coordinate;

   static_assert(not cjdb::Integral<source_coordinate::column_type>);
   static_assert(not cjdb::Integral<source_coordinate::line_type>);
   static_assert(not cjdb::Same<source_coordinate::column_type, source_coordinate::line_type>);
   static_assert(cjdb::Regular<source_coordinate>);

   SUBCASE("checks default constructed source_coordinate")
   {
      constexpr auto cursor = source_coordinate{};
      static_assert(cursor.line() == source_coordinate::line_type{1});
      static_assert(cursor.column() == source_coordinate::column_type{1});
   }

   SUBCASE("checks explicitly constructed source_coordinate")
   {
      constexpr auto column = 42;
      constexpr auto line = 35;

      constexpr auto cursor = source_coordinate{
         source_coordinate::line_type{line}, source_coordinate::column_type{column}};
      static_assert(cursor.line() == source_coordinate::line_type{line});
      static_assert(cursor.column() == source_coordinate::column_type{column});
   }

   SUBCASE("checks source_coordinate models EqualityComparable")
   {
      SUBCASE("checks == is an equivalence relation")
      {
         constexpr auto x =
            source_coordinate{source_coordinate::line_type{6}, source_coordinate::column_type{5}};

         static_assert(x == x);

         constexpr auto y = x;
         static_assert(x == y);
         static_assert(y == x);

         constexpr auto z = y;
         static_assert(z == y);
         static_assert(z == x);

         static_assert(not(x != y));
      }

      SUBCASE("checks when column and line are different")
      {
         constexpr auto first =
            source_coordinate{source_coordinate::line_type{40}, source_coordinate::column_type{30}};

         constexpr auto second =
            source_coordinate{source_coordinate::line_type{first.line() * first.line()},
               source_coordinate::column_type{first.column() * first.column()}};

         static_assert(first.column() != second.column());
         static_assert(first.line() != second.line());
         static_assert(first != second);
         static_assert(second != first);
      }

      SUBCASE("checks when column same, line different")
      {
         constexpr auto column = 30;

         constexpr auto first = source_coordinate{
            source_coordinate::line_type{column}, source_coordinate::column_type{column}};

         constexpr auto second = source_coordinate{
            source_coordinate::line_type{column * 2}, source_coordinate::column_type{column}};

         static_assert(first.line() != second.line());
         static_assert(first.column() == second.column());
         static_assert(first != second);
      }

      SUBCASE("checks when column different, line same")
      {
         constexpr auto line = 30;

         constexpr auto first = source_coordinate{
            source_coordinate::line_type{line}, source_coordinate::column_type{line * 4}};

         constexpr auto second = source_coordinate{
            source_coordinate::line_type{line}, source_coordinate::column_type{line}};

         static_assert(first.column() != second.column());
         static_assert(first.line() == second.line());
         static_assert(first != second);
      }
   }

   SUBCASE("checks source_coordinate models StrictTotallyOrdered")
   {
      SUBCASE("checks < is imposes a total order")
      {
         SUBCASE("checks when line1 < line2")
         {
            constexpr auto first =
               source_coordinate{source_coordinate::line_type{4}, source_coordinate::column_type{75}};
            constexpr auto second =
               source_coordinate{source_coordinate::line_type{6}, source_coordinate::column_type{32}};

            static_assert(first.line() < second.line());
            static_assert(first.column() > second.column());

            static_assert(first < second);
            static_assert(second > first);
            static_assert(first <= second);
            static_assert(second >= first);
         }

         SUBCASE("checks when line1 == line2 and column1 < column2")
         {
            constexpr auto line = 40;
            constexpr auto first = source_coordinate{
               source_coordinate::line_type{line}, source_coordinate::column_type{31}};

            constexpr auto second = source_coordinate{
               source_coordinate::line_type{line}, source_coordinate::column_type{87}};

            static_assert(first.line() == second.line());
            static_assert(first.column() < second.column());

            static_assert(first < second);
            static_assert(second > first);
            static_assert(first <= second);
            static_assert(second >= first);
         }

         SUBCASE("checks when coordinate1 == coordinate2")
         {
            constexpr auto first = source_coordinate{
               source_coordinate::line_type{42}, source_coordinate::column_type{32}};
            constexpr auto second = first;

            static_assert(first == second);
            static_assert(first <= second);
            static_assert(second >= first);
         }
      }
   }

   SUBCASE("Check source_coordinate motion is correct")
   {
      constexpr auto xcol = 5;
      constexpr auto xline = 32;
      constexpr auto x =
         source_coordinate{source_coordinate::line_type{xline}, source_coordinate::column_type{xcol}};

      SUBCASE("When y.column() == 0")
      {
         constexpr auto ycol = 0;
         constexpr auto yline = 76;
         constexpr auto y = source_coordinate{
            source_coordinate::line_type{yline}, source_coordinate::column_type{ycol}};

         static_assert(x != y);
         static_assert(y.column() == source_coordinate::column_type{0});

         constexpr auto result = x + y;
         constexpr auto expected = source_coordinate{
            source_coordinate::line_type{xline + yline}, source_coordinate::column_type{1}};
         static_assert(result == expected);
      }

      SUBCASE("When y.line() == 0")
      {
         constexpr auto ycol = 25;
         constexpr auto yline = 0;
         constexpr auto y = source_coordinate{
            source_coordinate::line_type{yline}, source_coordinate::column_type{ycol}};

         static_assert(x != y);
         static_assert(y.line() == source_coordinate::line_type{0});

         constexpr auto result = x + y;
         constexpr auto expected = source_coordinate{
            source_coordinate::line_type{xline},
            source_coordinate::column_type{xcol + ycol},
         };
         static_assert(result == expected);
      }

      SUBCASE("When y.column() != 0 and y.line() != 0")
      {
         constexpr auto ycol = 32;
         constexpr auto yline = 23;
         constexpr auto y = source_coordinate{
            source_coordinate::line_type{yline}, source_coordinate::column_type{ycol}};

         constexpr auto result = x + y;
         constexpr auto expected = source_coordinate{
            source_coordinate::line_type{xline + yline}, source_coordinate::column_type{ycol}};
         static_assert(result == expected);
      }
   }

   SUBCASE("Check ostream operators are correct")
   {
      SUBCASE("Check line")
      {
         constexpr auto line = source_coordinate::line_type{32};
         auto const result = fmt::format("{}", line);
         CHECK(result == "32");
      }
      SUBCASE("Check column")
      {
         constexpr auto column = source_coordinate::column_type{28};
         auto const result = fmt::format("{}", column);
         CHECK(result == "28");
      }
      SUBCASE("Check source_coordinate")
      {
         constexpr auto cursor =
            source_coordinate{source_coordinate::line_type{10}, source_coordinate::column_type{4}};

         auto const result = fmt::format("{}", cursor);
         CHECK(result == "10:4");
      }
   }
}
