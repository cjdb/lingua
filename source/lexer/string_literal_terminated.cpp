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
#include "lingua/utility/contract.hpp"
#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/empty.hpp>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/subrange.hpp>
#include <utility>

using namespace ranges;
namespace {
   subrange<std::u8string_view::const_iterator>
   string_literal_data(std::u8string_view const string_literal) noexcept
   {
      LINGUA_EXPECTS(not empty(string_literal));

      auto quote = find(string_literal, u8'"');
      auto const reversed = subrange{next(quote), end(string_literal)} | view::reverse;
      auto result = find(reversed, u8'"').base();
      return result == end(reversed).base() or *prev(result, 2) == u8'\\'
         ? subrange{end(string_literal), end(string_literal)}
         : subrange{quote, result};
   }
} // namespace

namespace lingua {
   bool string_literal_terminated(std::u8string_view const string_literal) noexcept
   {
      auto string_data = string_literal_data(string_literal);
      if (empty(string_data)) {
         return false;
      }
      else if (begin(string_literal) == begin(string_data)) {
         return end(string_literal) == end(string_data);
      }
      else {
         auto const raw_string_prefix = count(begin(string_literal), begin(string_data), u8'#');
         auto const raw_string_suffix = count(end(string_data), end(string_literal), u8'#');
         return raw_string_prefix == raw_string_suffix;
      }
   }
} // namespace lingua
