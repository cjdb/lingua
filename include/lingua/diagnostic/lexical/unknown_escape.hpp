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
#ifndef LINGUA_DIAGNOSTIC_DIAGNOSTIC_BAD_ESCAPE_HPP
#define LINGUA_DIAGNOSTIC_DIAGNOSTIC_BAD_ESCAPE_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/lexer/is_escape.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <cassert>
#include <fmt/format.h>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/distance.hpp>
#include <range/v3/size.hpp>
#include <range/v3/to_container.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/subrange.hpp>
#include <string_view>

namespace lingua {
   namespace detail_unknown_escape {
      enum class unknown_escape_kind { ascii, byte, unicode };
      using ranges::begin, ranges::end, ranges::distance;

      template<unknown_escape_kind kind>
      class unknown_escape_impl
      : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
         using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
         using u8string_view = std::u8string_view;
      public:
         using base_t::coordinates;
         using base_t::help_message;
         using base_t::level;

         explicit unknown_escape_impl(u8string_view const lexeme,
            ranges::subrange<u8string_view::iterator> const escape,
            source_coordinate_range const coordinates) noexcept
            : base_t{
               coordinates,
               format_diagnostic(
                  (LINGUA_EXPECTS(distance(lexeme) >= 4),
                   lexeme),
                  (LINGUA_EXPECTS(distance(escape) >= 2),
                   LINGUA_EXPECTS(*begin(escape) == u8'\\'),
                   u8string_view{begin(escape), static_cast<u8string_view::size_type>(distance(escape))}))
            }
         {
            auto const data = u8string_view{
               begin(escape),
               static_cast<u8string_view::size_type>(distance(escape))
            };
            if constexpr (kind == unknown_escape_kind::ascii) {
               LINGUA_EXPECTS(not lingua::is_ascii_escape(data));
            }
            else if constexpr (kind == unknown_escape_kind::byte) {
               LINGUA_EXPECTS(not lingua::is_byte_escape(data));
            }
            else {
               LINGUA_EXPECTS(not lingua::is_unicode_escape(data));
            }
         }

      private:
         static std::u8string format_diagnostic(u8string_view const lexeme,
            u8string_view const unrecognised_escape) noexcept
         {
            using namespace ranges;

            auto top_line = fmt::format(u8"unrecognised {} escape '{}' in string literal `", kind,
               unrecognised_escape);
            auto escape_highlight = u8'^' + std::u8string(size(unrecognised_escape) - 1, u8'~');
            // there might be multiple bad escapes in a single string, so we might need some space
            // padding between the first occurrence and where we're actually reporting
            auto const padding_size =
                 static_cast<std::size_t>(distance(begin(lexeme), begin(unrecognised_escape)))
               + size(top_line);
            auto padding = std::u8string(padding_size, u8' ');
            auto bottom_line = fmt::format(u8"{}{}", std::move(padding), std::move(escape_highlight));
            return fmt::format(u8"{}{}`\n{}", std::move(top_line), lexeme, std::move(bottom_line));
         }
      };
   } // namespace detail_unknown_escape

   using unknown_escape_ascii =
      detail_unknown_escape::unknown_escape_impl<detail_unknown_escape::unknown_escape_kind::ascii>;
   using unknown_escape_byte =
      detail_unknown_escape::unknown_escape_impl<detail_unknown_escape::unknown_escape_kind::byte>;
   using unknown_escape_unicode =
      detail_unknown_escape::unknown_escape_impl<detail_unknown_escape::unknown_escape_kind::unicode>;
} // namespace lingua

namespace fmt {
   template<>
   struct formatter<lingua::detail_unknown_escape::unknown_escape_kind, char8_t> {
      template<class ParseContext>
      constexpr auto parse(ParseContext& c) noexcept
      { return c.begin(); }

      template<class FormatContext>
      constexpr auto
      format(lingua::detail_unknown_escape::unknown_escape_kind const kind, FormatContext& c) noexcept
      {
         switch (kind) {
         case lingua::detail_unknown_escape::unknown_escape_kind::ascii:
            return ::fmt::format_to(c.out(), u8"ASCII");
         case lingua::detail_unknown_escape::unknown_escape_kind::byte:
            return ::fmt::format_to(c.out(), u8"byte");
         case lingua::detail_unknown_escape::unknown_escape_kind::unicode:
            return ::fmt::format_to(c.out(), u8"Unicode");
         default:
            LINGUA_ASSERT(false and u8"internal compiler error: uknown kind of unknown_escape_kind");
            std::abort();
         }
      }
   };
} // namespace fmt

#endif // LINGUA_DIAGNOSTIC_DIAGNOSTIC_BAD_ESCAPE_HPP
