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
#ifndef LINGUA_LEXER_STRING_LITERAL_TERMINATED_HPP
#define LINGUA_LEXER_STRING_LITERAL_TERMINATED_HPP

#include <string_view>

namespace lingua {
   /// \brief Determines if a string literal is correctly delimited.
   /// \param string_literal the string literal to check.
   /// \returns true if the string literal is a correctly delimited Rust string literal, false
   ///          otherwise.
   ///
   [[nodiscard]] bool string_literal_terminated(std::u8string_view const string_literal);
   // [[expects: not empty(string_literal)]];
} // namespace lingua

#endif // LINGUA_LEXER_STRING_LITERAL_TERMINATED_HPP
