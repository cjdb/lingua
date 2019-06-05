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
#ifndef LINGUA_DIAGNOSTIC_DETAIL_DIAGNOSTIC_BASE_HPP
#define LINGUA_DIAGNOSTIC_DETAIL_DIAGNOSTIC_BASE_HPP

#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include <string>
#include <string_view>
#include <utility>

namespace lingua::detail_diagnostic {
   template<diagnostic_level level_value>
   class diagnostic_base {
   public:
      static inline constexpr auto level = level_value;

      explicit diagnostic_base(source_coordinate_range const coordinates,
         std::u8string help_message) noexcept
         : coordinates_{coordinates}
         , help_message_{std::move(help_message)}
      {}

      source_coordinate_range coordinates() const noexcept
      { return coordinates_; }

      std::u8string_view help_message() const noexcept
      { return help_message_; }

   protected:
      ~diagnostic_base() = default;

   private:
      source_coordinate_range coordinates_;
      std::u8string help_message_;
   };
} // namespace lingua::detail_diagnostic

#endif // LINGUA_DIAGNOSTIC_DETAIL_DIAGNOSTIC_BASE_HPP
