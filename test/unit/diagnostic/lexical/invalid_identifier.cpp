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
#include "lingua/diagnostic/lexical/invalid_identifier.hpp"

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua_test/make_coordinates.hpp"
#include <doctest.h>

void check_invalid_identifier(std::u8string_view const identifier) noexcept
{
   using lingua::invalid_identifier;

   auto const coordinates = lingua_test::make_coordinates(identifier);
   auto const diagnostic = invalid_identifier{identifier, coordinates};
   CHECK(diagnostic.level == lingua::diagnostic_level::ill_formed);
   CHECK(diagnostic.coordinates() == coordinates);

   auto const expected_help_message = fmt::format(u8"`{}` is not allowed as a raw identifier.",
      identifier);
   CHECK(diagnostic.help_message() == expected_help_message);
}

TEST_CASE("checks that the invalid identifier diagnostic is correct") {
   check_invalid_identifier(u8"r#crate");
   check_invalid_identifier(u8"r#extern");
   check_invalid_identifier(u8"r#self");
   check_invalid_identifier(u8"r#super");
   check_invalid_identifier(u8"r#Self");
}
