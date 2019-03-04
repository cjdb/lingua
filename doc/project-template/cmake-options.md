# CMake options

`project_template` supports a handful of out-of-the-box options for configuring your project. These
options can be found in `cmake/project_template-options.cmake`.

# Changing the project name

**`project_template` uses `${PROJECT_NAME}_` to prefix all of its command-line options. This page will
use the term `project_template` as a substitute for _your_ project's name in every command-line option.
Unless your project is also called `project_template`, you should change this to the name of your
project.**

Since CMake can't magically know the name of your project, it requires a bit of input from you. In
the top-level `CMakeLists.txt` file, you'll see a line reading:

```cmake
project("project_template" CXX)
```

Change `project_template` to the name of your project. If your project's name has spaces in it, replace
the spaces with either dashes (`-`) or underscores (`_`).

# Options for clang-tidy

clang-tidy is enabled by default, but its usage can be toggled using `project_template_ENABLE_CLANG_TIDY`.
For example, if you wanted to turn clang-tidy off (not recommended), you'd run:

```bash
cmake path/to/project_template -Dproject_template_ENABLE_CLANG_TIDY=Off
```

`project_template` assumes, by default, that your instance of clang-tidy is installed as
`/usr/bin/clang-tidy`. If you want to use a specific version of clang-tidy, your clang-tidy is
installed in a different path, or you're not running on a Unix-compatible filesystem (e.g. Windows),
you'll want to change this using `project_template_CLANG_TIDY_PATH`:

```bash
cmake path/to/project_template -Dproject_template_CLANG_TIDY_PATH=path/to/clang-tidy
```

# Sanitisers

`project_template` offers two modes for enabling sanitisers: required and optional.

* A _required sanitiser_ is one that must be available to the compiler. If it is not available, then
  CMake will not generate a build system for you. Because MSVC doesn't support sanitisers, no
  sanitisers are required by default. To customise this, you can set the `project_template_REQUIRED_SANITIZERS`
  option.
* An _optional sanitiser_ does not need to be available to the compiler. If the compiler can use it,
  then it is enabled; if it isn't available, then it simply won't be enabled. You will receive
  output letting you know that it hasn't been enabled. AddressSanitizer, UndefinedBehaviorSanitizer,
  and ControlFlowIntegrity are the default optional sanitisers. Custom optional sanitiation is
  controlled using `project_template_OPTIONAL_SANITIZERS`.

There are certain restrictions when using sanitisers: not all sanitisers are compatible. As a
result, `project_template` will check that you haven't simultaneously listed any incompatible
sanitisers. **This means that you should always set both `project_template_REQUIRED_SANITIZERS` and
`project_template_OPTIONAL_SANITIZERS` together.**

## Sanitiser options

Both options support the same set of parameters: a semicolon-separated list of sanitisers. The
supported choices include:

* An empty string
* `Address`
* `Thread`
* `Memory`
* `MemoryWithOrigins`
* `Undefined`
* `DataFlow`
* `Leak`
* `ControlFlowIntegrity`
* `ShadowCallStack`
* `SafeStack`

In a project that requires AddressSanitizer and UndefinedBehaviorSanitizer, and supports
ControlFlowIntegrity, you would run the CMake configuration as if by:

```bash
cmake path/to/project_template                                \
   -Dproject_template_REQUIRED_SANITIZERS='Address;Undefined' \
   -Dproject_template_OPTIONAL_SANITIZERS='ControlFlowIntegrity'
```

## Incompatible sanitisers

Some sanitisers are incompatible with others. The table below documents which sanitisers can be
enabled with others. The ✔️ indicates 'compatible', the ❌ indicates 'incompatible', and the 🤷
indicates 'not yet tested'. This table uses `Memory` to indicate both the `Memory` option and the
`MemoryWithOrigins` option.

<table>
   <tr>
      <th></th>
      <th>Address</th>
      <th>Thread</th>
      <th>Memory</th>
      <th>Undefined</th>
      <th>DataFlow</th>
      <th>Leak</th>
      <th>ControlFlowIntegrity</th>
      <th>ShadowCallStack</th>
      <th>SafeStack</th>
   </tr>
   <tr>
      <th>Address</th>
      <th>-</th>
      <th>❌</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>❌</th>
      <th>✔️</th>
      <th>❌</th>
      <th>❌</th>
   </tr>
   <tr>
      <th>Thread</th>
      <th>❌</th>
      <th>-</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>❌</th>
      <th>✔️</th>
      <th>❌</th>
      <th>❌</th>
   </tr>
   <tr>
      <th>Memory</th>
      <th>❌</th>
      <th>❌</th>
      <th>-</th>
      <th>✔️</th>
      <th>❌</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>❌</th>
   </tr>
   <tr>
      <th>Undefined</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>-</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
   </tr>
   <tr>
      <th>DataFlow</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>-</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
   </tr>
   <tr>
      <th>Leak</th>
      <th>❌</th>
      <th>❌</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>-</th>
      <th>✔️</th>
      <th>❌</th>
      <th>❌</th>
   </tr>
   <tr>
      <th>ControlFlowIntegrity</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>-</th>
      <th>✔️</th>
      <th>✔️</th>
   </tr>
   <tr>
      <th>ShadowCallStack</th>
      <th>❌</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>❌</th>
      <th>✔️</th>
      <th>-</th>
      <th>❌</th>
   </tr>
   <tr>
      <th>SafeStack</th>
      <th>❌</th>
      <th>❌</th>
      <th>❌</th>
      <th>✔️</th>
      <th>✔️</th>
      <th>❌</th>
      <th>✔️</th>
      <th>❌</th>
      <th>-</th>
   </tr>
</table>

As an example, the following configuration will fail, and you won't get a build system:

```bash
cmake path/to/project_template                      \
   -Dproject_template_REQUIRED_SANITIZERS='Address' \
   -Dproject_template_OPTIONAL_SANITIZERS='Memory'
```

# Code coverage

Code coverage is supported using `project_template_CODE_COVERAGE`. It is not enabled by default, as
there is no option that is supported by all the toolchains. The available options include:

* `Off` (All)
* `gcov` (GCC, LLVM)
* `LLVMSourceCoverage` (LLVM only)
* `SanitizerCoverage` (not yet implemented)

# Adding your own options

You might want to add your own options for your own project. For Boolean options, you can use the
CMake [`option` declarator][cmake-option]. `project_template` also provides a function called
`project_template_enumerated_option` (don't rename this one!) that lets you specify your own options
when you'd like there to be more than just two valid inputs.


For example, if we wanted to set an option that took the version of LLVM that we're building
against, we might do it like so:

```cmake
project_template_enumerated_option(
   OPTION_NAME ${PROJECT_NAME}_LLVM_VERSION
   DESCRIPTION "Sets the version of LLVM to build against."
   TYPE STRING
   EXPECTS 4 5 6 7 8
   DEFAULT_VALUE 8
)
```

Here, we've created an option called `project_template_LLVM_VERSION` (`project_template` here is derived
from the CMake variable `${PROJECT_NAME}`), which accepts the strings `"4"`, `"5"`, `"6"`, `"7"`,
and `"8"`. If the user doesn't specify a value, then it defaults to `"8"`.

Supposing that we didn't wish to set a default version, but we also don't want the generation to
fail if a user doesn't provide a version of LLVM (perhaps they aren't building with LLVM at all),
then we can alter the above so that an empty string is also a valid option:

```cmake
project_template_enumerated_option(
   OPTION_NAME ${PROJECT_NAME}_LLVM_VERSION
   DESCRIPTION "Sets the version of LLVM to build against."
   TYPE STRING
   EXPECTS "" 4 5 6 7 8
)
```

In this case, `DEFAULT_VALUE` is the empty string, which is now a valid option.

Finally, suppose that we permit building against _multiple_ versions of LLVM with the same generated
build system. In this case, we'd need to let users provide a semicolon-separated list to CMake. We
can achive this using:

```cmake
project_template_enumerated_option(
   OPTION_NAME ${PROJECT_NAME}_LLVM_VERSION
   DESCRIPTION "Sets the version of LLVM to build against."
   TYPE STRING LIST
   EXPECTS "" 4 5 6 7 8
)
```

Here, we've added the term `LIST` after `TYPE STRING`. This indicates that the enumerated option
accepts a semicolon-separated list of expected values.

[cmake-option]: https://cmake.org/cmake/help/latest/command/option.html
[sanitizer-coverage-trace-pc-guard]: https://clang.llvm.org/docs/SanitizerCoverage.html#id2
[sanitizer-coverage-trace-pc]: https://clang.llvm.org/docs/SanitizerCoverage.html#tracing-pcs
