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
#include "lingua/diagnostic/lexical/unterminated_comment.hpp"

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <fmt/format.h>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <string_view>

TEST_CASE("checks the error type for unterminated comments") {
   using lingua::unterminated_comment;
   using namespace std::string_view_literals;

   constexpr auto help_message = u8"unterminated multi-line comment starting with:\n"
                                 u8"\t{}\n"
                                 u8"\t[note: multi-line comments in Rust may nest]";
   SUBCASE("Non-nested comments") {

      SUBCASE("Checks comment spanning a single line") {
         constexpr auto comment = u8"/* is this the real life?"sv;
         constexpr auto coordinates = lingua_test::make_coordinates(comment);
         auto const diagnostic = unterminated_comment{comment, coordinates};
         CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
         CHECK(diagnostic.coordinates() == coordinates);

         auto const expected_message = fmt::format(help_message, comment);
         CHECK(diagnostic.help_message() == expected_message);
      }

      SUBCASE("Checks comment spanning multiple lines") {
         constexpr auto comment = u8R"(/**
                                        * This is the comment that never ends,
                                        * It just goes on, and on, my friend.
                                        *
                                        * Some compilers started reading it,
                                        * Not knowing what it was.
                                        *
                                        * But they issued diagnostics, because...
                                        * (developer note: from the top))"sv;
         constexpr auto coordinates = lingua_test::make_coordinates(comment);
         auto const diagnostic = unterminated_comment{comment, coordinates};

         CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
         CHECK(diagnostic.coordinates() == coordinates);

         auto first_line = ranges::subrange{ranges::begin(comment), ranges::find(comment, u8'\n')}
                         | ranges::to<std::u8string>;
         auto const expected_message = fmt::format(help_message, std::move(first_line));
         CHECK(diagnostic.help_message() == expected_message);
      }
   }

   SUBCASE("Checks nested comments") {
      SUBCASE("Checks a nested comment on the same line") {
         constexpr auto comment = u8"/* this /* is a complete comment in C++ */"sv;
         constexpr auto coordinates = lingua_test::make_coordinates(comment);
         auto const diagnostic = unterminated_comment{comment, coordinates};

         CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
         CHECK(diagnostic.coordinates() == coordinates);

         auto const expected_message = fmt::format(help_message, comment);
         CHECK(diagnostic.help_message() == expected_message);
      }
   }
}
