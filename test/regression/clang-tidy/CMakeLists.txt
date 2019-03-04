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
include(lingua-add-targets-impl)

# Tests for checks that are disabled
add_project_template_library_impl(FILENAME -cert-dcl21-cpp.cpp)

add_project_template_library_impl(
   FILENAME -cert-dcl59-cpp.cpp
   PREFIX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

add_project_template_library_impl(
   FILENAME -google-build-namespaces.cpp
   INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})

add_project_template_library_impl(FILENAME -google-build-using-namespace.cpp)

add_project_template_library_impl(FILENAME -google-runtime-int.cpp)

add_project_template_library_impl(FILENAME -google-runtime-references.cpp)

add_project_template_library_impl(
   FILENAME -readability-named-parameter.cpp
   LINKAGE test.regression.clang-tidy.-readability-uppercase-literal-suffix)

add_project_template_library_impl(
   FILENAME -readability-uppercase-literal-suffix.cpp
   LIBRARY_TYPE STATIC)
