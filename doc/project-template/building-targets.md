# Building targets

## Overview

There are three main functions for using `project_template` to generate a build system. Those include:
   * `${PROJECT_NAME}_add_executable`
   * `${PROJECT_NAME}_add_library`
   * `${PROJECT_NAME}_add_test`

The term `${PROJECT_NAME}` should be what you've named your project in `CMakeLists.txt`. For the
purposes of this page, we'll assume that our project has been named `sunshine_tracker`. In this
case, the functions are called:
   * `add_sunshine_tracker_executable`
   * `add_sunshine_tracker_library`
   * `add_sunshine_tracker_test`

`add_sunshine_tracker_executable` is a wrapper for
[CMake's `add_executable` function][cmake-add_executable], with custom peripheral functionality.
`add_sunshine_tracker_library` is a wrapper for [CMake's `add_library` function][cmake-add_library],
with the same custom peripheral functionality. `add_sunshine_tracker_test` calls
`add_sunshine_tracker_executable`, and then automatically adds a [CMake test][cmake-add_test].

## Interface

The interface for all three is the same. We use EBNF syntax to define function usage. That is:

* `[x]` indicates zero or one occurrence of `x`
* `{x}` indicates one or more repetitions of `x`
* `x|y` indicates "choose exactly one of `x` or `y`".

```bash
${PROJECT_NAME}_add_X(
   FILENAME filename
   [LIBRARY_TYPE STATIC|SHARED|MODULE|OBJECT]

   [PRIVATE_PREFIX_INCLUDE {include-directories}]
   [INTERFACE_PREFIX_INCLUDE {include-directories}]
   [PUBLIC_PREFIX_INCLUDE {include-directories}]

   [PRIVATE_INCLUDE {include-directories}]
   [INTERFACE_INCLUDE {include-directories}]
   [PUBLIC_INCLUDE {include-directories}]

   [PRIVATE_COMPILER_DEFINITIONS {macros}]
   [INTERFACE_COMPILER_DEFINITIONS {macros}]
   [PUBLIC_COMPILER_DEFINITIONS {macros}]

   [PRIVATE_PREFIX_COMPILER_OPTIONS {compiler-options}]
   [INTERFACE_PREFIX_COMPILER_OPTIONS {compiler-options}]
   [PUBLIC_PREFIX_COMPILER_OPTIONS {compiler-options}]

   [PRIVATE_COMPILER_OPTIONS {compiler-options}]
   [INTERFACE_COMPILER_OPTIONS {compiler-options}]
   [PUBLIC_COMPILER_OPTIONS {compiler-options}]

   [PRIVATE_COMPILER_FEATURES {cmake-compiler-features}]
   [INTERFACE_COMPILER_FEATURES {cmake-compiler-features}]
   [PUBLIC_COMPILER_FEATURES {cmake-compiler-features}]

   [PRIVATE_LINKAGE {targets}]
   [INTERFACE_LINKAGE {targets}]
   [PUBLIC_LINKAGE {targets}]
)
```

```
FILENAME filename
```

Specifies the file to compile.

```
[LIBRARY_TYPE STATIC|SHARED|MODULE|OBJECT]
```

**Note: this option is only consumed by `add_sunshine_tracker_library`.**

Determines the kind of library that is being built. See the [CMake documentation for normal
libraries][cmake-add-library-normal-libraries] and the [CMake documentation for object
libraries][cmake-add-library-object-libraries].

```
[PRIVATE_PREFIX_INCLUDE {include-directories}]
[INTERFACE_PREFIX_INCLUDE {include-directories}]
[PUBLIC_PREFIX_INCLUDE {include-directories}]

[PRIVATE_INCLUDE {include-directories}]
[INTERFACE_INCLUDE {include-directories}]
[PUBLIC_INCLUDE {include-directories}]
```

Specifies a list of directories to be considered as include directories for the target. The
top-level `include/` is always an include directory. `<SCOPE>_PREFIX_INCLUDE` will add a list of
include directories to the _before_ the top-level `include/`; `<SCOPE>_INCLUDE` will add a list of
directories _after_ `include/`.

The order of inclusion is always processed as public, private, interface. For more information,
please see the [CMake documentation for `target_include_directories`][cmake-target-include-directories].

```
[PRIVATE_COMPILER_DEFINITIONS {macros}]
[INTERFACE_COMPILER_DEFINITIONS {macros}]
[PUBLIC_COMPILER_DEFINITIONS {macros}]
```

Specifies a list of macros to be defined when building the target.

The order of definitions is always processed as public, private, interface. For more information,
please see the [CMake documentation for `target_compile_definitions`][cmake-target-compile-definitions].

```
[PRIVATE_PREFIX_COMPILER_OPTIONS {compiler-options}]
[INTERFACE_PREFIX_COMPILER_OPTIONS {compiler-options}]
[PUBLIC_PREFIX_COMPILER_OPTIONS {compiler-options}]

[PRIVATE_COMPILER_OPTIONS {compiler-options}]
[INTERFACE_COMPILER_OPTIONS {compiler-options}]
[PUBLIC_COMPILER_OPTIONS {compiler-options}]
```

Specifies a list of compiler options to turn on when compiling the target. There are a [list of
default build options][relative-default-build-options]. `<SCOPE>_PREFIX_COMPILER_OPTIONS` will add
a list of options _before_ the default list of options; `<SCOPE>_COMPILER_OPTIONS` will add a list
of options _after_ the default list of options.

For more information, please see the [CMake documentation for
`target_compile_options`][cmake-target-compile-options].

```
[PRIVATE_COMPILER_FEATURES {cmake-compiler-features}]
[INTERFACE_COMPILER_FEATURES {cmake-compiler-features}]
[PUBLIC_COMPILER_FEATURES {cmake-compiler-features}]
```

Specifies a list of [CMake compiler features][cmake-target-compile-features] to enable when
compiling the target.

```
[PRIVATE_LINKAGE {targets}]
[INTERFACE_LINKAGE {targets}]
[PUBLIC_LINKAGE {targets}]
```

Specifies a list of targets to link. This can either be a target name from your project, or an
external project's name.

For more information, see the [CMake documentation for `target_link_libraries`][cmake-target-link-libraries].

## Result

The result of these functions is ultimately a target named after its path, until the file's
extension, with all slashes being converted to dots. To keep target names portable, short, and
unique, the path root is cropped from `${CMAKE_SOURCE_DIR}`, which means that if you are building
`/tmp/kittens/sunshine-tracker/source/solar-flares/mild.cpp`, the target's name would be
`source.solar-flares.mild`.

## Default include directory

By default, `project_template` includes only the top-level directory `include/` and any include
directories indicated by CMake's `find_package`.

## Build options

`${PROJECT_NAME}_add_X` builds targets with many options enabled. The philosophy behind this is that
it is significantly easier for something to exist if it exists by default, rather than having
someone opt into using it.

### Warnings

Except where expressly noted, all warnings are compiled as errors.

#### Warnings for GCC

When compiling with GCC, the following warnings are enabled by default:

* `-Wall`
* `-Wextra`
* `-Wcast-align`
* `-Wconversion`
* `-Wdouble-promotion`
* `-Wnon-virtual-dtor`
* `-Wold-style-cast`
* `-Woverloaded-virtual`
* `-Wpedantic`
* `-Wshadow`
* `-Wsign-conversion`
* `-Wsign-promo`
* `-Wunused`
* `-Wformat=2`
* `-Wodr`
* `-Wnull-dereference` (GCC 6+ only)

The following warnings are disabled by default:

* `-Wno-attributes`

The GNU documentation for warnings describes the functionality for all of the above:

* [GCC 5 warning options][gcc-5-warnings]
* [GCC 6 warning options][gcc-6-warnings]
* [GCC 7 warning options][gcc-7-warnings]
* [GCC 8 warning options][gcc-8-warnings]

#### Warnings for Clang

When compiling with Clang, the following warnings are enabled by default:

* [`-Weverything`][llvm-Weverything]

The following warnings are disabled by default:

* [`-Wno-unused-command-line-argument`][llvm-Wno-unused-command-line-argument]
* [`-Wno-missing-prototypes`][llvm-Wno-missing-prototypes]
* [`-Wno-c++98-compat`][llvm-Wno-c++98-compat]
* [`-Wno-c++98-compat-pedantic`][llvm-Wno-c++98-compat-pedantic]
* [`-Wno-padded`][llvm-Wno-padded]

The [LLVM documentation for warnings][llvm-warnings] goes into exquisite detail about their many
warnings.

#### Warnings for MSVC

The following warnings are enabled by default when compiling with MSVC:

* [`/W4`][msvc-w4]
* [`/w14242`][msvc-w14242]
* [`/w14254`][msvc-w14254]
* [`/w14263`][msvc-w14263]
* [`/w14265`][msvc-w14265]
* [`/w14287`][msvc-w14287]
* [`/we4289`][msvc-we4289]
* [`/w14296`][msvc-w14296]
* [`/w14311`][msvc-w14311]
* [`/w14545`][msvc-w14545]
* [`/w14546`][msvc-w14546]
* [`/w14547`][msvc-w14547]
* [`/w14549`][msvc-w14549]
* [`/w14555`][msvc-w14555]
* [`/w14619`][msvc-w14619]
* [`/w14640`][msvc-w14640]
* [`/w14826`][msvc-w14826]
* [`/w14905`][msvc-w14905]
* [`/w14906`][msvc-w14906]
* [`/w14928`][msvc-w14928]

#### Pedantic mode

All of GCC, Clang, and MSVC, offer what is known as a 'pedantic' mode. This is enabled by default
for all three compilers.

* GCC: `-pedantic` (see GCC warnings for documentation)
* [Clang: `-pedantic`][llvm-pedantic]
* [MSVC: `/permissive-`][msvc-permissive]

### Program instrumentation

* [GCC 5 instrumentation documentation][gcc-5-instrumentation]
* [GCC 6 instrumentation documentation][gcc-6-instrumentation]
* [GCC 7 instrumentation documentation][gcc-7-instrumentation]
* [GCC 8 instrumentation documentation][gcc-8-instrumentation]

#### Buffer overflow protection

`project_template` offers support for [buffer overflow protection][wiki-buffer-overflow-protection].
[Microsoft enable this in their compiler by default][msvc-GS], so `project_template` only concerns
itself with enabling buffer overflow protection for GCC and Clang.

By default, `project_template` enables `-fstack-protector` for all builds, and `-fstack-protector-all`
for debug builds.

If you are unsure about when to enable or disable buffer overflow protection, consider reading this
[StackOverflow answer][stack-overflow] and this [Microsoft article][msvc-buffer-overflow-blog].

#### Sanitisers

`add_sunshine_tracker_X` builds with all sanitsers specified by
`SUNSHINE_TRACKER_REQUIRED_SANITIZERS` and `SUNSHINE_TRACKER_OPTIONAL_SANITIZERS`.

The [`project_template` sanitisers documentation][project-template-sanitizers] covers this in more detail.

#### Code coverage

If code coverage is enabled by the user, then the [documented code coverage
flags][project-template-code-coverage] will be added to the mix.

### Visibility support

`project_template` has both GCC and Clang compile using `-fvisibility=default` by default. GNU
articulates why [visibility support is important][gcc-visibility-support]. LLVM also discusses
[visibility options][llvm-visibility].

### Macros

As `project_template` ships with `range-v3/v1.0-beta`, targets are compiled as if
`#define RANGES_DEEP_STL_INTEGRATION 1` is present in the code. As this is a package-dependent
definition, it is defined in `cmake/project_template-packages.cmake`.

[project-template-code-coverage]: https://www.cjdb.com.au/404.html
[project-template-sanitizers]: https://www.cjdb.com.au/404.html

[cmake-add-library-normal-libraries]: https://cmake.org/cmake/help/latest/command/add_library.html#normal-libraries
[cmake-add-library-object-libraries]: https://cmake.org/cmake/help/latest/command/add_library.html#object-libraries
[cmake-target-compile-definitions]: https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
[cmake-target-compile-features]: https://cmake.org/cmake/help/latest/command/target_compile_features.html
[cmake-target-compile-options]: https://cmake.org/cmake/help/latest/command/target_compile_options.html
[cmake-target-include-directories]: https://cmake.org/cmake/help/latest/command/target_include_directories.html
[cmake-target-link-libraries]: https://cmake.org/cmake/help/v3.0/command/target_link_libraries.html

[gcc-5-instrumentation]: https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Instrumentation-Options.html
[gcc-6-instrumentation]: https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Instrumentation-Options.html
[gcc-7-instrumentation]: https://gcc.gnu.org/onlinedocs/gcc-7.4.0/gcc/Instrumentation-Options.html
[gcc-8-instrumentation]: https://gcc.gnu.org/onlinedocs/gcc-8.3.0/gcc/Instrumentation-Options.html
[gcc-5-warnings]: https://gcc.gnu.org/onlinedocs/gcc-5.5.0/gcc/Warning-Options.html
[gcc-6-warnings]: https://gcc.gnu.org/onlinedocs/gcc-6.5.0/gcc/Warning-Options.html
[gcc-7-warnings]: https://gcc.gnu.org/onlinedocs/gcc-7.4.0/gcc/Warning-Options.html
[gcc-8-warnings]: https://gcc.gnu.org/onlinedocs/gcc-8.3.0/gcc/Warning-Options.html
[gcc-visibility-support]: https://gcc.gnu.org/wiki/Visibility

[llvm-pedantic]: https://clang.llvm.org/docs/UsersManual.html#options-to-control-error-and-warning-messages
[llvm-stack-protector]: https://clang.llvm.org/docs/ClangCommandLineReference.html#cmdoption-clang-fstack-protector
[llvm-visibility]: https://clang.llvm.org/docs/LTOVisibility.html
[llvm-warnings]: https://clang.llvm.org/docs/DiagnosticsReference.html
[llvm-Weverything]: https://clang.llvm.org/docs/UsersManual.html#diagnostics-enable-everything
[llvm-Wno-unused-command-line-argument]: https://clang.llvm.org/docs/DiagnosticsReference.html#wunused-command-line-argument
[llvm-Wno-missing-prototypes]: https://clang.llvm.org/docs/DiagnosticsReference.html#wmissing-prototypes
[llvm-Wno-c++98-compat]: https://clang.llvm.org/docs/DiagnosticsReference.html#wc-98-compat
[llvm-Wno-c++98-compat-pedantic]: https://clang.llvm.org/docs/DiagnosticsReference.html#wc-98-compat-pedantic
[llvm-Wno-padded]: https://clang.llvm.org/docs/DiagnosticsReference.html#wpadded

[msvc-buffer-overflow-blog]: https://msdn.microsoft.com/en-us/magazine/mt814419.aspx
[msvc-GS]: https://docs.microsoft.com/en-us/cpp/build/reference/gs-buffer-security-check?view=vs-2017
[msvc-permissive]: https://docs.microsoft.com/en-us/cpp/build/reference/permissive-standards-conformance?view=vs-2017
[msvc-w4]: https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=vs-2017
[msvc-w14242]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4242?view=vs-2017
[msvc-w14254]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4254?view=vs-2017
[msvc-w14263]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4263?view=vs-2017
[msvc-w14265]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4265?view=vs-2017
[msvc-w14287]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4287?view=vs-2017
[msvc-we4289]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4289?view=vs-2017
[msvc-w14296]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4296?view=vs-2017
[msvc-w14311]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4311?view=vs-2017
[msvc-w14545]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4545?view=vs-2017
[msvc-w14546]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4546?view=vs-2017
[msvc-w14547]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4547?view=vs-2017
[msvc-w14549]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4549?view=vs-2017
[msvc-w14555]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4555?view=vs-2017
[msvc-w14619]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4619?view=vs-2017
[msvc-w14640]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4640?view=vs-2017
[msvc-w14826]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warnings-c4800-through-c4999?view=vs-2017
[msvc-w14905]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4905?view=vs-2017
[msvc-w14906]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4906?view=vs-2017
[msvc-w14928]: https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4928?view=vs-2017

[stack-overflow]: https://stackoverflow.com/questions/1629685/when-and-how-to-use-gccs-stack-protection-feature

[wiki-buffer-overflow-protection]: https://en.wikipedia.org/wiki/Buffer_overflow_protection
