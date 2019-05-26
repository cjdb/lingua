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
#ifndef LINGUA_UTILITY_CONTRACT_HPP
#define LINGUA_UTILITY_CONTRACT_HPP

// clang-format off

#include <fmt/format.h>
#include <stdexcept>


#define LINGUA_EXPECTS(...) LINGUA_CONTRACT_IMPL("pre-condition", __VA_ARGS__)

#define LINGUA_ASSERT(...) LINGUA_CONTRACT_IMPL("assertion", __VA_ARGS__)

#define LINGUA_ENSURES(LINGUA_RESULT, ...)                              \
   LINGUA_CONTRACT_IMPL("post-condition", __VA_ARGS__), LINGUA_RESULT   \


#define LINGUA_CONTRACT_IMPL(LINGUA_KIND, ...)                             \
   [](bool const result) constexpr noexcept {                              \
      if (not result) {                                                    \
         _Pragma("GCC diagnostic push")                                    \
            _Pragma("GCC diagnostic ignored \"-Wterminate\"")              \
            throw std::logic_error{                                        \
               fmt::format("{} `{}` failed on line {} in file {}",         \
                  LINGUA_KIND, #__VA_ARGS__, LINGUA_TO_STRING(__LINE__),   \
                  LINGUA_TO_STRING(__FILE__))                              \
            };                                                             \
         _Pragma("GCC diagnostic pop")                                     \
      }                                                                    \
   }(static_cast<bool>(__VA_ARGS__))                                       \



#define LINGUA_TO_STRING(LINGUA_STRING) LINGUA_TO_STRING_IMPL(LINGUA_STRING)
#define LINGUA_TO_STRING_IMPL(LINGUA_STRING) #LINGUA_STRING

#define LINGUA_AXIOM(_) true

#ifdef LINGUA_ENABLE_AUDITNG_CONTACTS
#   define LINGUA_AUDIT(LINGUA_CONTRACT_PARAMETER) LINGUA_CONTRACT_PARAMETER
#else
#   define LINGUA_AUDIT(_) true
#endif // LINGUA_ENABLE_AUDIT

// clang-format on

#endif // LINGUA_UTILITY_CONTRACT_HPP
