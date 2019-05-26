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
#ifndef LINGUA_SOURCE_COORDINATE_HPP
#define LINGUA_SOURCE_COORDINATE_HPP

#include "lingua/utility/contract.hpp"
#include <cjdb/utility/strong_value.hpp>
#include <cstdint>
#include <fmt/format.h>
#include <tuple>

namespace lingua {
   class [[nodiscard]] source_coordinate
   {
      struct column_tag {};
      struct line_tag {};

   public:
      using value_type = std::intmax_t;
      using column_type = cjdb::strong_value<value_type, column_tag>;
      using line_type = cjdb::strong_value<value_type, line_tag>;

      /// \brief Initialises the object so that it is equivalent to
      ///        source_coordinate{line_type{1}, column_type{1}}.
      ///
      constexpr source_coordinate() = default;

      /// \brief Initialises the column value with the column parameter and initialises the line
      ///        value with the line parameter.
      ///
      constexpr explicit source_coordinate(line_type const line, column_type const column) noexcept
         : line_{(LINGUA_EXPECTS(line >= line_type{0}), line)}
         , column_{(LINGUA_EXPECTS(column >= column_type{0}), column)}
      {}

      /// \brief Returns the column value.
      ///
      constexpr column_type column() const noexcept
      { return column_; }

      /// \brief Returns the line value.
      ///
      constexpr line_type line() const noexcept
      { return line_; }

      /// \brief Checks that the column and line values of x are the same as the column and line
      ///        values of y.
      ///
      constexpr friend bool operator==(source_coordinate const x, source_coordinate const y) noexcept
      { return std::tie(x.column_, x.line_) == std::tie(y.column_, y.line_); }

      /// \brief Checks that the column and line values of x are not the same as the column and line
      ///        values of y.
      ///
      constexpr friend bool operator!=(source_coordinate const x, source_coordinate const y) noexcept
      { return not(x == y); }

      /// \brief Checks that a source_coordinate is strictly less than another source_coordinate.
      /// \param x A source_coordinate to be checked.
      /// \param y A source_coordinate to be checked.
      /// \returns true if:
      ///     * `x.line()` is less than `y.line()`
      ///     * `x.line() == y.line()` and `x.column()` is less than `y.column()`
      ///  false otherwise
      ///
      constexpr friend bool operator<(source_coordinate const x, source_coordinate const y) noexcept
      { return x.line() < y.line() or (x.line() == y.line() and x.column() < y.column()); }

      /// \brief Checks that a source_coordinate is strictly greater than another source_coordinate.
      /// \param x A source_coordinate to be checked.
      /// \param y A source_coordinate to be checked.
      /// \returns `y < x`
      ///
      constexpr friend bool operator>(source_coordinate const x, source_coordinate const y) noexcept
      { return y < x; }

      /// \brief Checks that a source_coordinate is partially less than another source_coordinate.
      /// \param x A source_coordinate to be checked.
      /// \param y A source_coordinate to be checked.
      /// \returns `not (y < x)`
      ///
      constexpr friend bool operator<=(source_coordinate const x, source_coordinate const y) noexcept
      { return not(y < x); }

      /// \brief Checks that a source_coordinate is partially less than another source_coordinate.
      /// \param x A source_coordinate to be checked.
      /// \param y A source_coordinate to be checked.
      /// \returns `not (x < y)`
      ///
      constexpr friend bool operator>=(source_coordinate const x, source_coordinate const y) noexcept
      { return not(x < y); }

      /// \brief Moves x by:
      ///       1. adding y.line() to x.line(), and
      ///       2. (a) adding y.column() to x.column() if y.line() == 0, or
      ///          (b) assigning y.column() to x.column() otherwise
      ///
      constexpr friend source_coordinate operator+(source_coordinate const x,
         source_coordinate const y) noexcept
      {
         return source_coordinate{
            line_type{x.line() + y.line()},
            column_type{
               line_type{0} == y.line() ? x.column() + y.column() : column_type{0} < y.column()
                                        ? y.column() : column_type{1}
            }
         };
      }

   private:
      line_type line_{1};
      column_type column_{1};
   };
} // namespace lingua

namespace fmt {
   template<>
   struct formatter<lingua::source_coordinate::line_type> {
      template<class Context>
      constexpr auto parse(Context& c) noexcept
      { return c.begin(); }

      template<class Context>
      constexpr auto format(lingua::source_coordinate::line_type const line, Context& c) noexcept
      { return ::fmt::format_to(c.begin(), "{}", static_cast<std::intmax_t>(line)); }
   };

   template<>
   struct formatter<lingua::source_coordinate::column_type> {
      template<class Context>
      constexpr auto parse(Context& c) noexcept
      { return c.begin(); }

      template<class Context>
      constexpr auto format(lingua::source_coordinate::column_type const column, Context& c) noexcept
      { return ::fmt::format_to(c.begin(), "{}", static_cast<std::intmax_t>(column)); }
   };

   template<>
   struct formatter<lingua::source_coordinate> {
      template<class Context>
      constexpr auto parse(Context& c) noexcept
      { return c.begin(); }

      template<class Context>
      constexpr auto format(lingua::source_coordinate const coordinate, Context& c) noexcept
      { return ::fmt::format_to(c.begin(), "{}:{}", coordinate.line(), coordinate.column()); }
   };
} // namespace fmt

#endif // LINGUA_SOURCE_COORDINATE_HPP
