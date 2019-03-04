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

# \brief Builds an executable.
# \param prefix A string that prefixes the filename that will be removed from its path. Everything
#               that prefixes the prefix will _also_ be removed.
# \param file The name of the source file.
#
function(${PROJECT_NAME}_add_executable)
   add_project_template_executable_impl(${ARGN})
endfunction()

# \brief Builds a library.
# \param prefix A string that prefixes the filename that will be removed from its path. Everything
#               that prefixes the prefix will _also_ be removed.
# \param file The name of the source file.
#
function(${PROJECT_NAME}_add_library)
   add_project_template_library_impl(${ARGN})
endfunction()



# \brief Builds a test executable and creates a test target (for CTest).
# \param prefix A string that prefixes the filename that will be removed from its path. Everything
#               that prefixes the prefix will _also_ be removed.
# \param file The name of the source file.
#
function(${PROJECT_NAME}_add_test)
   add_project_template_test_impl(${ARGN})
endfunction()
