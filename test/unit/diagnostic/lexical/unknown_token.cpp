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
#include "lingua/diagnostic/lexical/unknown_token.hpp"

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>
#include <fmt/format.h>

void check_unknown_token(std::u8string_view const token)
{
   auto const coordinates = lingua_test::make_coordinates(token);
   auto const diagnostic = lingua::unknown_token{token, coordinates};
   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_message = fmt::format(u8R"(unknown token "{}")", token);
   CHECK(diagnostic.help_message() == expected_message);
}

TEST_CASE("checks the error type for unknown tokens") {
   using lingua::unknown_token;
   using namespace std::string_view_literals;

   check_unknown_token(u8"`");
   check_unknown_token(u8"~");
   check_unknown_token(u8"££");
}
