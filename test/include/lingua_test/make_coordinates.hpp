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
#ifndef LINGUA_UNIT_TEST_MAKE_COORDINATES_HPP
#define LINGUA_UNIT_TEST_MAKE_COORDINATES_HPP

#include "lingua/source_coordinate.hpp"
#include "lingua/source_coordinate_range.hpp"
#include <range/v3/distance.hpp>
#include <string_view>

namespace lingua_test {
   using namespace lingua;

   constexpr source_coordinate_range make_coordinates(std::string_view const lexeme) noexcept
   {
      return source_coordinate_range{
         source_coordinate{
            source_coordinate::line_type{1},
            source_coordinate::column_type{1}
         },
         source_coordinate{
            source_coordinate::line_type{1},
            source_coordinate::column_type{ranges::distance(lexeme)}
         }
      };
   }
} // namespace lingua_test

#endif // LINGUA_UNIT_TEST_MAKE_COORDINATES_HPP
