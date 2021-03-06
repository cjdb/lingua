#
#  Copyright Christopher Di Bella
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# System packages
if (LINGUA_ENABLE_CLANG_TIDY)
   find_package(Clang REQUIRED)
   find_package(ClangTidy REQUIRED)
endif()
find_package(CodeCoverage REQUIRED)
include(lingua-sanitizers)

# Library packages
find_package(doctest REQUIRED)

find_package(range-v3 REQUIRED)
add_compile_options(-DRANGES_DEEP_STL_INTEGRATION=1)

find_package(cjdb REQUIRED)
find_package(expected REQUIRED)
find_package(fmt REQUIRED)
