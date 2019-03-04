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

function(add_impl_libraries target scope to_link)
   if(to_link)
      message(STATUS "Linking `${to_link}` with ${scope} scope, against ${target}.")
      target_link_libraries("${target}" ${scope} "${to_link}")
   endif()
endfunction()

function(add_impl_prefix_include target scope to_include)
   if(to_include)
      message(STATUS
         "Prefixing `${to_include}` to include directories, with ${scope} scope, when compiling ${target}.")
      target_include_directories("${target}" ${scope} "${to_include}")
   endif()
endfunction()

function(add_impl_suffix_include target scope to_include)
   if(to_include)
      message(STATUS
         "Suffixing `${to_include}` to include directories, with ${scope} scope, when compiling ${target}.")
      target_include_directories("${target}" ${scope} "${to_include}")
   endif()
endfunction()

function(add_impl_prefix_options target scope to_prefix)
   if(to_prefix)
      message(STATUS
         "Prefixing compiler option(s) `${to_prefix}` with ${scope} scope, when compiling ${target}.")
      target_compile_options("${target}" BEFORE ${scope} "${to_prefix}")
   endif()
endfunction()

function(add_impl_suffix_options target scope to_suffix)
   if(to_suffix)
      message(STATUS
         "Suffixing compiler option(s) `${to_suffix}` with ${scope} scope, when compiling ${target}.")
      target_compile_options("${target}" ${scope} "${to_suffix}")
   endif()
endfunction()

function(add_impl_enable_features target scope to_enable)
   if(to_enable)
      message(STATUS
         "Enabling compiler feature(s) `${to_enable}` with ${scope} scope, when compiling ${target}.")
      target_compile_features("${target}" ${scope} "${to_enable}")
   endif()
endfunction()

function(add_impl_define_macros target scope to_define)
   if(to_define)
      message(STATUS
         "Defining macro(s) `${to_define}` with ${scope} scope, when compiling ${target}")
      target_compile_definitions("${target}" ${scope} "${to_define}")
   endif()
endfunction()

function(add_impl_custom_target_settings)
   set(optional_values "")
   set(single_value_args TARGET SCOPE)
   set(multi_value_args LINK SUFFIX_INCLUDE SUFFIX_OPTIONS ENABLE_FEATURE DEFINITIONS)
   cmake_parse_arguments(
      settings
      "${optional_values}"
      "${single_value_args}"
      "${multi_value_args}"
      ${ARGN})

   add_impl_suffix_include("${settings_TARGET}" "${settings_SCOPE}" "${settings_SUFFIX_INCLUDE}")
   add_impl_suffix_options("${settings_TARGET}" "${settings_SCOPE}" "${settings_SUFFIX_OPTIONS}")
   add_impl_enable_features("${settings_TARGET}" "${settings_SCOPE}" "${settings_ENABLE_FEATURES}")
   add_impl_define_macros("${settings_TARGET}" "${settings_SCOPE}" "${settings_DEFINITIONS}")
   add_impl_libraries("${settings_TARGET}" "${settings_SCOPE}" "${settings_LINK}")
endfunction()

macro(PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS_LIBRARIES)
   set(optional_values "")
   set(single_value_args TARGET)
   set(multi_value_args

   PREFIX_INCLUDE
   INTERFACE_PREFIX_INCLUDE
   PUBLIC_PREFIX_INCLUDE

      INCLUDE
      INTERFACE_INCLUDE
      PUBLIC_INCLUDE

      LIBRARIES
      INTERFACE_LIBRARIES
      PUBLIC_LIBRARIES

      PREFIX_COMPILER_OPTIONS
      INTERFACE_PREFIX_COMPILER_OPTIONS
      PUBLIC_PREFIX_COMPILER_OPTIONS

      COMPILER_OPTIONS
      INTERFACE_COMPILER_OPTIONS
      PUBLIC_COMPILER_OPTIONS

      COMPILER_FEATURES
      INTERFACE_COMPILER_FEATURES
      PUBLIC_COMPILER_FEATURES

      COMPILER_DEFINITIONS
      INTERFACE_COMPILER_DEFINITIONS
      PUBLIC_COMPILER_DEFINITIONS)

   cmake_parse_arguments(
      add_target_args
      "${optional_values}"
      "${single_value_args}"
      "${multi_value_args}"
      ${ARGN})
endmacro()

# \brief Provides build settings common for all targets.
# \param target The target to apply the target to.
#
function(add_impl)
   PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS_LIBRARIES(${ARGN})

   add_impl_prefix_include(
      "${add_target_args_TARGET}"
      PRIVATE
      "${add_target_args_PREFIX_INCLUDE}")
   add_impl_prefix_include(
      "${add_target_args_TARGET}"
      INTERFACE
      "${add_target_args_INTERFACE_PREFIX_INCLUDE}")
   add_impl_prefix_include(
      "${add_target_args_TARGET}"
      PUBLIC
      "${add_target_args_PUBLIC_PREFIX_INCLUDE}")

   add_impl_prefix_options(
      "${add_target_args_TARGET}"
      PRIVATE
      "${add_target_args_PREFIX_COMPILER_OPTIONS}")
   add_impl_prefix_options(
      "${add_target_args_TARGET}"
      INTERFACE
      "${add_target_args_INTERFACE_PREFIX_COMPILER_OPTIONS}")
   add_impl_prefix_options(
      "${add_target_args_TARGET}"
      PUBLIC
      "${add_target_args_PUBLIC_PREFIX_COMPILER_OPTIONS}")

   # Warnings
   target_compile_options("${add_target_args_TARGET}" PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:
         /analyze
         /W4
         /w14242 # 'identfier': conversion from 'type1' to 'type1', possible loss of data
         /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
         /w14263 # 'function': member function does not override any base class virtual member function
         /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
         /w14287 # 'operator': unsigned/negative constant mismatch
         /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
         /w14296 # 'operator': expression is always 'boolean_value'
         /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
         /w14545 # expression before comma evaluates to a function which is missing an argument list
         /w14546 # function call before comma missing argument list
         /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
         /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
         /w14555 # expression has no effect; expected expression with side-effect
         /w14619 # pragma warning: there is no warning number 'number'
         /w14640 # Enable warning on thread un-safe static member initialization
         /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
         /w14905 # wide string literal cast to 'LPSTR'
         /w14906 # string literal cast to 'LPWSTR'
         /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
         >
      $<$<CXX_COMPILER_ID:GNU>:
         -Wall
         -Wextra
         -Wcast-align
         -Wconversion
         -Wdouble-promotion
         -Wnon-virtual-dtor
         -Wold-style-cast
         -Woverloaded-virtual
         -Wpedantic
         -Wshadow
         -Wsign-conversion
         -Wsign-promo
         -Wunused
         -Wformat=2
         -Wodr
         -Wno-attributes
         $<$<NOT:$<VERSION_LESS:${CMAKE_CXX_COMPILER_VERSION},6>>:
            -Wnull-dereference>
         >
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
         -Weverything
         -Wno-unused-command-line-argument
         -Wno-missing-prototypes
         -Wno-c++98-compat
         -Wno-c++98-compat-pedantic
         -Wno-padded>)
   # Non-warning compiler options
   target_compile_options("${add_target_args_TARGET}" PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:
         /WX            # Warnings as errors
         /permissive-   # Strict C++ mode
      >
      $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
         -Werror                    # Warnings as errors
         -pedantic                  # Strict C++ mode
         -fdiagnostics-color=always # Get good diagnostic highlighting
         -fvisibility=default       # Default visibility mode
         -fstack-protector          # Buffer overflow protection
         $<$<CONFIG:Debug>:
            -fstack-protector-all>  # Stronger buffer overflow protection
      >)

   target_include_directories("${add_target_args_TARGET}" PUBLIC "${PROJECT_SOURCE_DIR}/include")

   if(${${PROJECT_NAME}_CODE_COVERAGE})
      target_link_libraries("${add_target_args_TARGET}" PRIVATE CodeCoverage::all)
   endif()

   target_link_libraries("${add_target_args_TARGET}" PRIVATE
      $<$<OR:$<CONFIG:Debug>,$<BOOL:${${PROJECT_NAME}_SANITIZE_RELEASE}>>:Sanitizer::all>
      $<$<AND:$<NOT:$<CONFIG:Debug>>,$<BOOL:${Sanitizer_ControlFlowIntegrity_FOUND}>>:Sanitizer::ControlFlowIntegrity>)

   if((${Sanitizer_Memory_FOUND}) OR (${Sanitizer_MemoryWithOrigins_FOUND}))
      target_compile_options(
         "${add_target_args_TARGET}" PRIVATE
         $<$<OR:$<CONFIG:Debug>,$<BOOL:${${PROJECT_NAME}_SANITIZE_RELEASE}>>:
            -fno-omit-frame-pointer -fno-optimize-sibling-calls
            $<$<BOOL:${Sanitizer_MemoryWithOrigins_FOUND}>:
               -fsanitize-memory-track-origins=2>>)
   endif()

   add_impl_custom_target_settings(
      TARGET         "${add_target_args_TARGET}"
      SCOPE PRIVATE
      SUFFIX_INCLUDE "${add_target_args_INCLUDE}"
      SUFFIX_OPTIONS "${add_target_args_COMPILER_OPTIONS}"
      ENABLE_FEATURE "${add_target_args_COMPILER_FEATURES}"
      DEFINITIONS    "${add_target_args_COMPILER_DEFINITIONS}"
      LINK           "${add_target_args_LIBRARIES}")
   add_impl_custom_target_settings(
      TARGET         "${add_target_args_TARGET}"
      SCOPE PUBLIC
      SUFFIX_INCLUDE "${add_target_args_PUBLIC_INCLUDE}"
      SUFFIX_OPTIONS "${add_target_args_PUBLIC_COMPILER_OPTIONS}"
      ENABLE_FEATURE "${add_target_args_PUBLIC_COMPILER_FEATURES}"
      DEFINITIONS    "${add_target_args_PUBLIC_COMPILER_DEFINITIONS}"
      LINK           "${add_target_args_PUBLIC_LIBRARIES}")
   add_impl_custom_target_settings(
      TARGET         "${add_target_args_TARGET}"
      SCOPE INTERFACE
      SUFFIX_INCLUDE "${add_target_args_INTERFACE_INCLUDE}"
      SUFFIX_OPTIONS "${add_target_args_INTERFACE_COMPILER_OPTIONS}"
      ENABLE_FEATURE "${add_target_args_INTERFACE_COMPILER_FEATURES}"
      DEFINITIONS    "${add_target_args_INTERFACE_COMPILER_DEFINITIONS}"
      LINK           "${add_target_args_INTERFACE_LIBRARIES}")
endfunction()

# \brief Produces a target name for compiling a translation unit.
# \param prefix A string that prefixes the filename that will be removed from its path. Everything
#               that prefixes the prefix will _also_ be removed.
# \param file The name of the source file.
# \returns A sans-prefix path that is dot separated.
#
function(name_target filename)
   get_filename_component(sublibrary "${filename}" ABSOLUTE)
   string(REGEX REPLACE "[.][^.]+$" "" sublibrary "${sublibrary}")
   string(REPLACE "/" "." sublibrary "${sublibrary}")
   string(REPLACE "/" "." remove_prefix "${CMAKE_SOURCE_DIR}")
   string(REGEX REPLACE "^.*${remove_prefix}[.]" "" sublibrary "${sublibrary}")
   set(target "${sublibrary}" PARENT_SCOPE)
endfunction()

macro(PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS)
   set(optional_values "")
   set(single_value_args FILENAME LIBRARY_TYPE)
   set(multi_value_args

      PUBLIC_PREFIX_INCLUDE
      PREFIX_INCLUDE
      INTERFACE_PREFIX_INCLUDE

      PUBLIC_INCLUDE
      INCLUDE
      INTERFACE_INCLUDE

      LIBRARIES
      PUBLIC_LIBRARIES
      INTERFACE_LIBRARIES

      PUBLIC_PREFIX_COMPILER_OPTIONS
      PREFIX_COMPILER_OPTIONS
      INTERFACE_PREFIX_COMPILER_OPTIONS

      PUBLIC_COMPILER_OPTIONS
      COMPILER_OPTIONS
      INTERFACE_COMPILER_OPTIONS

      PUBLIC_COMPILER_FEATURES
      COMPILER_FEATURES
      INTERFACE_COMPILER_FEATURES

      PUBLIC_COMPILER_DEFINITIONS
      COMPILER_DEFINITIONS
      INTERFACE_COMPILER_DEFINITIONS)

   cmake_parse_arguments(
      add_target_args
      "${optional_values}"
      "${single_value_args}"
      "${multi_value_args}"
      ${ARGN})

   if(${add_target_args_FILENAME} STREQUAL "")
      PROJECT_TEMPLATE_MULTILINE_STRING(
         error
         "FILENAME is not set. Cannot build a target without a filename.")
      message(FATAL_ERROR "${error}")
   endif()
endmacro()

macro(PROJECT_TEMPLATE_CALL_ADD_IMPL)
   add_impl(
      TARGET "${target}"

      PREFIX_INCLUDE "${add_target_args_PREFIX_INCLUDE}"
      INTERFACE_PREFIX_INCLUDE "${add_target_args_INTERFACE_PREFIX_INCLUDE}"
      PUBLIC_PREFIX_INCLUDE "${add_target_args_PUBLIC_PREFIX_INCLUDE}"

      INCLUDE "${add_target_args_INCLUDE}"
      INTERFACE_INCLUDE "${add_target_args_INTERFACE_INCLUDE}"
      PUBLIC_INCLUDE "${add_target_args_PUBLIC_INCLUDE}"

      LIBRARIES "${add_target_args_LIBRARIES}"
      INTERFACE_LIBRARIES "${add_target_args_INTERFACE_LIBRARIES}"
      PUBLIC_LIBRARIES "${add_target_args_PUBLIC_LIBRARIES}"

      PREFIX_COMPILER_OPTIONS "${add_target_args_PREFIX_COMPILER_OPTIONS}"
      INTERFACE_PREFIX_COMPILER_OPTIONS "${add_target_args_INTERFACE_PREFIX_COMPILER_OPTIONS}"
      PUBLIC_PREFIX_COMPILER_OPTIONS "${add_target_args_PUBLIC_PREFIX_COMPILER_OPTIONS}"

      COMPILER_OPTIONS "${add_target_args_COMPILER_OPTIONS}"
      INTERFACE_COMPILER_OPTIONS "${add_target_args_INTERFACE_COMPILER_OPTIONS}"
      PUBLIC_COMPILER_OPTIONS "${add_target_args_PUBLIC_COMPILER_OPTIONS}"

      COMPILER_FEATURES "${add_target_args_COMPILER_FEATURES}"
      INTERFACE_COMPILER_FEATURES "${add_target_args_INTERFACE_COMPILER_FEATURES}"
      PUBLIC_COMPILER_FEATURES "${add_target_args_PUBLIC_COMPILER_FEATURES}"

      COMPILER_DEFINITIONS "${add_target_args_COMPILER_DEFINITIONS}"
      INTERFACE_COMPILER_DEFINITIONS "${add_target_args_INTERFACE_COMPILER_DEFINITIONS}"
      PUBLIC_COMPILER_DEFINITIONS "${add_target_args_PUBLIC_COMPILER_DEFINITIONS}"
   )
endmacro()

# \see ${PROJECT_NAME}_add_executable
#
function(add_project_template_executable_impl)
   PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(${ARGN})

   name_target("${add_target_args_FILENAME}")
   add_executable("${target}" "${add_target_args_FILENAME}")
   PROJECT_TEMPLATE_CALL_ADD_IMPL()
endfunction()

# \see ${PROJECT_NAME}_add_library
#
function(add_project_template_library_impl)
   PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(${ARGN})

   name_target("${add_target_args_FILENAME}")

   set(legal_library_types "" STATIC SHARED MODULE OBJECT)
   list(FIND legal_library_types "${add_target_args_LIBRARY_TYPE}" library_type_result)
   if(${library_type_result} EQUAL -1)
      message(FATAL_ERROR "Cannot add a library of type \"${add_target_args_LIBRARY_TYPE}\"")
   endif()

   add_library("${target}" ${add_target_args_LIBRARY_TYPE} "${add_target_args_FILENAME}")
   PROJECT_TEMPLATE_CALL_ADD_IMPL()
endfunction()

# \see ${PROJECT_NAME}_add_test
#
function(add_project_template_test_impl)
   add_project_template_executable_impl(${ARGN})

   PROJECT_TEMPLATE_EXTRACT_ADD_TARGET_ARGS(${ARGN})
   name_target("${add_target_args_FILENAME}")
   add_test("test.${target}" "${target}")
endfunction()
