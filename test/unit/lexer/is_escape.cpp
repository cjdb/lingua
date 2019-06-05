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
#include "lingua/lexer/is_escape.hpp"

#include <cjdb/cctype/isxdigit.hpp>
#include <cjdb/cctype/toupper.hpp>
#include <doctest.h>
#include <fmt/format.h>
#include <limits>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/none_of.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

template<int escape_max, class F>
void check_is_escape_common(F const& is_escape) noexcept
{
   CHECK(is_escape(u8R"(\n)"));
   CHECK(is_escape(u8R"(\r)"));
   CHECK(is_escape(u8R"(\t)"));
   CHECK(is_escape(u8R"(\\)"));
   CHECK(is_escape(u8R"(\0)"));

   namespace view = ranges::view;
   for (auto i : view::iota(0, escape_max)) {
      auto const escape = fmt::format(u8R"(\x{:02x})", i);
      CHECK(is_escape(escape));
   }

   using namespace std::string_view_literals;
   auto not_ascii_escape = [escapes = u8"nrtx\\0"sv](auto const c) noexcept {
      return escapes.find(c) == std::u8string_view::npos;
   };
   auto non_escapes = view::iota(0, std::numeric_limits<char8_t>::max() + 1)
                    | view::transform([](auto const c) noexcept { return static_cast<char8_t>(c); })
                    | view::filter(not_ascii_escape)
                    | view::transform([](auto const c) { return fmt::format(u8R"(\{})", c); });
   for (auto const non_escape : non_escapes) {
      for (auto x : non_escape) {
         fmt::print("{}", static_cast<char>(x));
      }
      fmt::print("\n");
      CHECK(ranges::distance(non_escape) == 2);
      CHECK(not is_escape(non_escape));
   }
}

TEST_CASE("checks ASCII escapes are valid") {
   check_is_escape_common<128>(lingua::is_ascii_escape);
}

TEST_CASE("checks byte escapes are valid") {
   check_is_escape_common<256>(lingua::is_byte_escape);
}

template<int N>
class generate_strings {
   friend class generate_strings<N + 1>;
   static constexpr long long compute_permutations() noexcept
   {
      constexpr auto distance = '~' - ' ';
      if constexpr (N == 0) {
         static_assert(N != 0);
      }
      else if constexpr (N == 1) {
         return distance;
      }
      else {
         return distance + generate_strings<N - 1>::compute_permutations();
      }
   }
public:
   inline static constexpr auto prefixed_permutations = compute_permutations();

   auto operator()() const noexcept
   { return impl(); }

private:
   template<class... X>
   static auto impl(X&&... xs)
   {
      namespace view = ranges::view;
      if constexpr (sizeof...(xs) < N) {
         auto to_char8_t = [](auto const c) noexcept { return static_cast<char8_t>(c); };
         auto const printable_ascii = view::iota(static_cast<int>(' '), '~' + 1)
                                    | view::transform(to_char8_t);
         return impl(std::forward<X>(xs)..., printable_ascii);
      }
      else {
         auto make_string = []<class... Ts>(std::tuple<Ts...> const& x) noexcept {
            return fill_result(std::u8string(sizeof...(Ts), u8'z'), x,
               std::make_index_sequence<sizeof...(Ts)>{});
         };
         return view::cartesian_product(std::forward<X>(xs)...) | view::transform(make_string);
      }
   }

   template<class... Ts, std::size_t... Size>
   static std::u8string
   fill_result(std::u8string result, std::tuple<Ts...> const& x, std::index_sequence<Size...>) noexcept
   {
      (..., (result[Size] = std::get<Size>(x)));
      return result;
   }
};

template<int N>
void exhaustive_check_is_unicode_escape() noexcept
{
   using lingua::is_unicode_escape;
   namespace view = ranges::view;

   auto all_hex = [](std::u8string_view const x) noexcept {
      return ranges::all_of(x, cjdb::isxdigit);
   };
   auto format = view::transform([](auto&& x) noexcept {
      return fmt::format(u8R"(\u{{{}}})", std::forward<decltype(x)>(x));
   });
   auto no_close_braces = view::filter([](std::u8string_view const x) noexcept {
      return x.find(u8'}') == std::u8string_view::npos;
   });

   auto strings = ::generate_strings<N>{}() | no_close_braces;
   SUBCASE("valid escapes") {
      auto expected_escapes = strings | view::filter(all_hex) | format;
      CHECK(ranges::all_of(expected_escapes, is_unicode_escape));
   }
   SUBCASE("invalid escapes") {
      auto expected_non_escapes = strings | view::filter(std::not_fn(all_hex)) | format;
      CHECK(ranges::none_of(expected_non_escapes, is_unicode_escape));
   }
}


TEST_CASE("checks Unicode escapes are valid")
{
   exhaustive_check_is_unicode_escape<1>();
   exhaustive_check_is_unicode_escape<2>();
   exhaustive_check_is_unicode_escape<3>();

   SUBCASE("valid escapes") {
      using lingua::is_unicode_escape;

      SUBCASE("four digits") {
         CHECK(is_unicode_escape(u8"\\u{0000}"));
         CHECK(is_unicode_escape(u8"\\u{0001}"));
         CHECK(is_unicode_escape(u8"\\u{0002}"));
         CHECK(is_unicode_escape(u8"\\u{0003}"));
         CHECK(is_unicode_escape(u8"\\u{0004}"));
         CHECK(is_unicode_escape(u8"\\u{0005}"));
         CHECK(is_unicode_escape(u8"\\u{0006}"));
         CHECK(is_unicode_escape(u8"\\u{0007}"));
         CHECK(is_unicode_escape(u8"\\u{0008}"));
         CHECK(is_unicode_escape(u8"\\u{0009}"));
         CHECK(is_unicode_escape(u8"\\u{000a}"));
         CHECK(is_unicode_escape(u8"\\u{000b}"));
         CHECK(is_unicode_escape(u8"\\u{000c}"));
         CHECK(is_unicode_escape(u8"\\u{000d}"));
         CHECK(is_unicode_escape(u8"\\u{000e}"));
         CHECK(is_unicode_escape(u8"\\u{000f}"));
         CHECK(is_unicode_escape(u8"\\u{000A}"));
         CHECK(is_unicode_escape(u8"\\u{000B}"));
         CHECK(is_unicode_escape(u8"\\u{000C}"));
         CHECK(is_unicode_escape(u8"\\u{000D}"));
         CHECK(is_unicode_escape(u8"\\u{000E}"));
         CHECK(is_unicode_escape(u8"\\u{000F}"));
         CHECK(is_unicode_escape(u8"\\u{2345}"));
         CHECK(is_unicode_escape(u8"\\u{00D5}"));
         CHECK(is_unicode_escape(u8"\\u{eeF1}"));
      }

      SUBCASE("five digits") {
         CHECK(is_unicode_escape(u8"\\u{00000}"));
         CHECK(is_unicode_escape(u8"\\u{00001}"));
         CHECK(is_unicode_escape(u8"\\u{00002}"));
         CHECK(is_unicode_escape(u8"\\u{00003}"));
         CHECK(is_unicode_escape(u8"\\u{00004}"));
         CHECK(is_unicode_escape(u8"\\u{00005}"));
         CHECK(is_unicode_escape(u8"\\u{00006}"));
         CHECK(is_unicode_escape(u8"\\u{00007}"));
         CHECK(is_unicode_escape(u8"\\u{00008}"));
         CHECK(is_unicode_escape(u8"\\u{00009}"));
         CHECK(is_unicode_escape(u8"\\u{0000a}"));
         CHECK(is_unicode_escape(u8"\\u{0000b}"));
         CHECK(is_unicode_escape(u8"\\u{0000c}"));
         CHECK(is_unicode_escape(u8"\\u{0000d}"));
         CHECK(is_unicode_escape(u8"\\u{0000e}"));
         CHECK(is_unicode_escape(u8"\\u{0000f}"));
         CHECK(is_unicode_escape(u8"\\u{0000A}"));
         CHECK(is_unicode_escape(u8"\\u{0000B}"));
         CHECK(is_unicode_escape(u8"\\u{0000C}"));
         CHECK(is_unicode_escape(u8"\\u{0000D}"));
         CHECK(is_unicode_escape(u8"\\u{0000E}"));
         CHECK(is_unicode_escape(u8"\\u{0000F}"));
         CHECK(is_unicode_escape(u8"\\u{12345}"));
         CHECK(is_unicode_escape(u8"\\u{F00D5}"));
         CHECK(is_unicode_escape(u8"\\u{BeeF1}"));
      }

      SUBCASE("six digits") {
         CHECK(is_unicode_escape(u8"\\u{000000}"));
         CHECK(is_unicode_escape(u8"\\u{000001}"));
         CHECK(is_unicode_escape(u8"\\u{000002}"));
         CHECK(is_unicode_escape(u8"\\u{000003}"));
         CHECK(is_unicode_escape(u8"\\u{000040}"));
         CHECK(is_unicode_escape(u8"\\u{000005}"));
         CHECK(is_unicode_escape(u8"\\u{000006}"));
         CHECK(is_unicode_escape(u8"\\u{000007}"));
         CHECK(is_unicode_escape(u8"\\u{000008}"));
         CHECK(is_unicode_escape(u8"\\u{000009}"));
         CHECK(is_unicode_escape(u8"\\u{00000a}"));
         CHECK(is_unicode_escape(u8"\\u{00000b}"));
         CHECK(is_unicode_escape(u8"\\u{00000c}"));
         CHECK(is_unicode_escape(u8"\\u{00000d}"));
         CHECK(is_unicode_escape(u8"\\u{00000e}"));
         CHECK(is_unicode_escape(u8"\\u{00000f}"));
         CHECK(is_unicode_escape(u8"\\u{00000A}"));
         CHECK(is_unicode_escape(u8"\\u{00000B}"));
         CHECK(is_unicode_escape(u8"\\u{00000C}"));
         CHECK(is_unicode_escape(u8"\\u{00000D}"));
         CHECK(is_unicode_escape(u8"\\u{00000E}"));
         CHECK(is_unicode_escape(u8"\\u{00000F}"));
         CHECK(is_unicode_escape(u8"\\u{123045}"));
         CHECK(is_unicode_escape(u8"\\u{F000D5}"));
         CHECK(is_unicode_escape(u8"\\u{BeeF10}"));
      }
   }

   SUBCASE("invalid escapes") {
      using lingua::is_unicode_escape;
      SUBCASE("four digits") {
         CHECK(not is_unicode_escape(u8"\\u{000G}"));
         CHECK(not is_unicode_escape(u8"\\u{00H1}"));
         CHECK(not is_unicode_escape(u8"\\u{0I12}"));
         CHECK(not is_unicode_escape(u8"\\u{J123}"));
         CHECK(not is_unicode_escape(u8"\\u{12V4}"));
         CHECK(not is_unicode_escape(u8"\\u{000L}"));
         CHECK(not is_unicode_escape(u8"\\u{000M}"));
         CHECK(not is_unicode_escape(u8"\\u{000N}"));
         CHECK(not is_unicode_escape(u8"\\u{000O}"));
         CHECK(not is_unicode_escape(u8"\\u{000P}"));
         CHECK(not is_unicode_escape(u8"\\u{000Q}"));
         CHECK(not is_unicode_escape(u8"\\u{000R}"));
         CHECK(not is_unicode_escape(u8"\\u{000S}"));
         CHECK(not is_unicode_escape(u8"\\u{000T}"));
         CHECK(not is_unicode_escape(u8"\\u{000U}"));
         CHECK(not is_unicode_escape(u8"\\u{000V}"));
         CHECK(not is_unicode_escape(u8"\\u{000W}"));
         CHECK(not is_unicode_escape(u8"\\u{000X}"));
         CHECK(not is_unicode_escape(u8"\\u{addY}"));
         CHECK(not is_unicode_escape(u8"\\u{000Z}"));
         CHECK(not is_unicode_escape(u8"\\u{000g}"));
         CHECK(not is_unicode_escape(u8"\\u{00h1}"));
         CHECK(not is_unicode_escape(u8"\\u{0i12}"));
         CHECK(not is_unicode_escape(u8"\\u{j123}"));
         CHECK(not is_unicode_escape(u8"\\u{&234}"));
         CHECK(not is_unicode_escape(u8"\\u{000l}"));
         CHECK(not is_unicode_escape(u8"\\u{000m}"));
         CHECK(not is_unicode_escape(u8"\\u{000n}"));
         CHECK(not is_unicode_escape(u8"\\u{000o}"));
         CHECK(not is_unicode_escape(u8"\\u{000p}"));
         CHECK(not is_unicode_escape(u8"\\u{000q}"));
         CHECK(not is_unicode_escape(u8"\\u{000r}"));
         CHECK(not is_unicode_escape(u8"\\u{000s}"));
         CHECK(not is_unicode_escape(u8"\\u{000t}"));
         CHECK(not is_unicode_escape(u8"\\u{000u}"));
         CHECK(not is_unicode_escape(u8"\\u{000v}"));
         CHECK(not is_unicode_escape(u8"\\u{000w}"));
         CHECK(not is_unicode_escape(u8"\\u{000x}"));
         CHECK(not is_unicode_escape(u8"\\u{addy}"));
         CHECK(not is_unicode_escape(u8"\\u{000z}"));
         CHECK(not is_unicode_escape(u8"\\u{<>#^}"));
      }

      SUBCASE("five digits") {
         CHECK(not is_unicode_escape(u8"\\u{0000G}"));
         CHECK(not is_unicode_escape(u8"\\u{000H1}"));
         CHECK(not is_unicode_escape(u8"\\u{00I12}"));
         CHECK(not is_unicode_escape(u8"\\u{0J123}"));
         CHECK(not is_unicode_escape(u8"\\u{K1234}"));
         CHECK(not is_unicode_escape(u8"\\u{0000L}"));
         CHECK(not is_unicode_escape(u8"\\u{0000M}"));
         CHECK(not is_unicode_escape(u8"\\u{0000N}"));
         CHECK(not is_unicode_escape(u8"\\u{0000O}"));
         CHECK(not is_unicode_escape(u8"\\u{0000P}"));
         CHECK(not is_unicode_escape(u8"\\u{0000Q}"));
         CHECK(not is_unicode_escape(u8"\\u{0000R}"));
         CHECK(not is_unicode_escape(u8"\\u{0000S}"));
         CHECK(not is_unicode_escape(u8"\\u{0000T}"));
         CHECK(not is_unicode_escape(u8"\\u{0000U}"));
         CHECK(not is_unicode_escape(u8"\\u{0000V}"));
         CHECK(not is_unicode_escape(u8"\\u{0000W}"));
         CHECK(not is_unicode_escape(u8"\\u{0000X}"));
         CHECK(not is_unicode_escape(u8"\\u{baddY}"));
         CHECK(not is_unicode_escape(u8"\\u{0000Z}"));
         CHECK(not is_unicode_escape(u8"\\u{0000g}"));
         CHECK(not is_unicode_escape(u8"\\u{000h1}"));
         CHECK(not is_unicode_escape(u8"\\u{00i12}"));
         CHECK(not is_unicode_escape(u8"\\u{0j123}"));
         CHECK(not is_unicode_escape(u8"\\u{k1&34}"));
         CHECK(not is_unicode_escape(u8"\\u{0000l}"));
         CHECK(not is_unicode_escape(u8"\\u{0000m}"));
         CHECK(not is_unicode_escape(u8"\\u{0000n}"));
         CHECK(not is_unicode_escape(u8"\\u{0000o}"));
         CHECK(not is_unicode_escape(u8"\\u{0000p}"));
         CHECK(not is_unicode_escape(u8"\\u{0000q}"));
         CHECK(not is_unicode_escape(u8"\\u{0000r}"));
         CHECK(not is_unicode_escape(u8"\\u{0000s}"));
         CHECK(not is_unicode_escape(u8"\\u{0000t}"));
         CHECK(not is_unicode_escape(u8"\\u{0000u}"));
         CHECK(not is_unicode_escape(u8"\\u{0000v}"));
         CHECK(not is_unicode_escape(u8"\\u{0000w}"));
         CHECK(not is_unicode_escape(u8"\\u{0000x}"));
         CHECK(not is_unicode_escape(u8"\\u{baddy}"));
         CHECK(not is_unicode_escape(u8"\\u{0000z}"));
         CHECK(not is_unicode_escape(u8"\\u{$<>#^}"));
      }

      SUBCASE("six digits") {
         CHECK(not is_unicode_escape(u8"\\u{00a00G}"));
         CHECK(not is_unicode_escape(u8"\\u{00a0H1}"));
         CHECK(not is_unicode_escape(u8"\\u{00aI12}"));
         CHECK(not is_unicode_escape(u8"\\u{0Ja123}"));
         CHECK(not is_unicode_escape(u8"\\u{K1a234}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00L}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00M}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00N}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00O}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00P}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00Q}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00R}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00S}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00T}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00U}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00V}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00W}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00X}"));
         CHECK(not is_unicode_escape(u8"\\u{baaddY}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00Z}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00g}"));
         CHECK(not is_unicode_escape(u8"\\u{00a0h1}"));
         CHECK(not is_unicode_escape(u8"\\u{00ai12}"));
         CHECK(not is_unicode_escape(u8"\\u{0ja123}"));
         CHECK(not is_unicode_escape(u8"\\u{k1a2&4}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00l}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00m}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00n}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00o}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00p}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00q}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00r}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00s}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00t}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00u}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00v}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00w}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00x}"));
         CHECK(not is_unicode_escape(u8"\\u{baaddy}"));
         CHECK(not is_unicode_escape(u8"\\u{00a00z}"));
         CHECK(not is_unicode_escape(u8"\\u{$<a>#^}"));
      }
   }
}
