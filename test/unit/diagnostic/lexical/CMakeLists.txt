#
#  Copyright Christopher Di Bella
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
lingua_add_test(
   FILENAME float_exponent_missing_digits.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME float_multiple_radix_points.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME invalid_identifier.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME unknown_escape.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3
      source.lexer.is_escape)

lingua_add_test(
   FILENAME unknown_digit.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME unknown_token.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME unterminated_comment.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      range-v3)

lingua_add_test(
   FILENAME unterminated_string_literal.cpp
   INCLUDE "${CMAKE_SOURCE_DIR}/test/include"
   COMPILER_DEFINITIONS
      DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
   LIBRARIES
      cjdb
      doctest::doctest
      fmt::fmt
      source.lexer.string_literal_terminated
      range-v3)
