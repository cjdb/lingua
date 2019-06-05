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
#ifndef LINGUA_LEXER_SOURCE_COORDINATE_RANGE_HPP
#define LINGUA_LEXER_SOURCE_COORDINATE_RANGE_HPP

#include "lingua/source_coordinate.hpp"
#include "lingua/utility/contract.hpp"
#include <fmt/format.h>
#include <range/v3/distance.hpp>
#include <tuple>

namespace lingua {
   class [[nodiscard]] source_coordinate_range
   {
   public:
      /// \brief Constructs a source_coordinate_range.
      /// \param begin The beginning of the source_coordinate range.
      /// \param end The end of the source_coordinate range.
      /// \note begin and end form the half-open interval [begin, end).
      ///
      explicit constexpr source_coordinate_range(source_coordinate const begin,
         source_coordinate const end) noexcept
         : begin_{(LINGUA_EXPECTS(begin <= end), begin)}
         , end_{end}
      {}

      /// \brief Returns the beginning of the source_coordinate range.
      /// \returns the beginning of the source_coordinate range.
      ///
      constexpr source_coordinate begin() const noexcept
      { return begin_; }

      /// \brief Returns the end of the source_coordinate range.
      /// \returns the end of the source_coordinate range.
      ///
      constexpr source_coordinate end() const noexcept
      { return end_; }

      /// \brief Checks if the range is empty.
      /// \returns true if `begin() == end()`; false otherwise
      ///
      constexpr bool empty() const noexcept
      { return begin() == end(); }

      /// \brief Checks that two source_coordinate_ranges are equivalent.
      /// \param x A source_coordinate_range to be checked.
      /// \param y A source_coordinate_range to be checked.
      /// \returns true if `x.begin() == y.begin()` and `x.end() == y.end()`; false otherwise.
      ///
      friend constexpr bool operator==(source_coordinate_range const& x, source_coordinate_range const& y) noexcept
      { return std::tie(x.begin_, x.end_) == std::tie(y.begin_, y.end_); }

      /// \brief Checks that two source_coordinate_ranges are not equivalent.
      /// \param x A source_coordinate_range to be checked.
      /// \param y A source_coordinate_range to be checked.
      /// \returns `not (x == y)`
      ///
      friend constexpr bool operator!=(source_coordinate_range const& x, source_coordinate_range const& y) noexcept
      { return not(x == y); }

   protected:
      constexpr void begin(source_coordinate x) noexcept
      { begin_ = x; }

      constexpr void end(source_coordinate x) noexcept
      { end_ = x; }

   private:
      source_coordinate begin_;
      source_coordinate end_;
   };
} // namespace lingua

namespace fmt {
   template<>
   struct formatter<lingua::source_coordinate_range, char8_t> {
      template<class ParseContext>
      constexpr auto parse(ParseContext& c) noexcept
      { return c.begin(); }

      template<class FormatContext>
      constexpr auto format(lingua::source_coordinate_range const& r, FormatContext& c) noexcept
      { return ::fmt::format_to(c.out(), u8"from {} to {}", r.begin(), r.end()); }
   };
} // namespace fmt

#endif // LINGUA_LEXER_SOURCE_COORDINATE_RANGE_HPP
