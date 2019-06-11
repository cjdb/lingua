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
#ifndef LINGUA_DIAGNOSTIC_LEXICAL_INVALID_IDENTIFIER_HPP
#define LINGUA_DIAGNOSTIC_LEXICAL_INVALID_IDENTIFIER_HPP

#include "lingua/diagnostic/detail/diagnostic_base.hpp"
#include "lingua/diagnostic/diagnostic_level.hpp"
#include "lingua/source_coordinate_range.hpp"
#include "lingua/utility/contract.hpp"
#include <array>
#include <fmt/format.h>
#include <range/v3/algorithm/any_of.hpp>

namespace lingua {
   class invalid_identifier
   : private detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed> {
      using base_t = detail_diagnostic::diagnostic_base<diagnostic_level::ill_formed>;
   public:
      using base_t::coordinates;
      using base_t::help_message;
      using base_t::level;

      explicit invalid_identifier(std::u8string_view identifier,
         source_coordinate_range const coordinates) noexcept
         : base_t{coordinates, generate_message(identifier)}
      {
         LINGUA_EXPECTS(identifier.starts_with(u8"r#"));
         LINGUA_EXPECTS(is_prohibited_identifier(identifier));
      }
   private:
      static std::u8string generate_message(std::u8string_view const identifier) noexcept
      { return fmt::format(u8"`{}` is not allowed as a raw identifier.", identifier); }

      static bool is_prohibited_identifier(std::u8string_view const identifier) noexcept
      {
         using namespace std::string_view_literals;
         constexpr auto expected = std::array{
              u8"crate"sv
            , u8"extern"sv
            , u8"self"sv
            , u8"super"sv
            , u8"Self"sv
         };
         return ranges::any_of(expected, [identifier](auto const c) {
            constexpr auto skip_prefix = u8"r#"sv;
            return c == identifier.substr(skip_prefix.size());
         });
      }
   };
} // namespace lingua

#endif // LINGUA_DIAGNOSTIC_LEXICAL_INVALID_IDENTIFIER_HPP
