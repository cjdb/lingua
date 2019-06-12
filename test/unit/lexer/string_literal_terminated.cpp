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
#include "lingua/lexer/string_literal_terminated.hpp"

#include <doctest.h>

TEST_CASE("checks that string literals are terminated") {
   SUBCASE("correctly-delimited strings") {
      CHECK(lingua::string_literal_terminated(u8R"("hello")"));
      CHECK(lingua::string_literal_terminated(u8R"("hello\"")"));
      CHECK(lingua::string_literal_terminated(u8R"(r"hello")"));
      CHECK(lingua::string_literal_terminated(u8R"(r#"hello"#)"));
      CHECK(lingua::string_literal_terminated(u8R"(r##"hello"##)"));
   }

   SUBCASE("incorrectly-delimited strings") {
      CHECK(not lingua::string_literal_terminated(u8R"("hello)"));
      CHECK(not lingua::string_literal_terminated(u8R"("hello\")"));
      CHECK(not lingua::string_literal_terminated(u8R"(r"hello)"));
      CHECK(not lingua::string_literal_terminated(u8R"(r#"hello")"));
      CHECK(not lingua::string_literal_terminated(u8R"(r#"hello#)"));
      CHECK(not lingua::string_literal_terminated(u8R"(r##"hello"#)"));
      CHECK(not lingua::string_literal_terminated(u8R"(r##"hello"###)"));
   }
}
