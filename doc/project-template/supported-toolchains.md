# Supported toolchains

`project_template` supports many toolchains. This page goes lists all of the supported toolchains and
their periphery at a high level. More details can be found on individual pages.

1. [Package manager][toc-pacman]
2. [Build system][toc-build-system]
3. [Compilers and Standard Libraries][toc-compilers]
   1. [MSVC][toc-msvc]
   2. [MSYS2][toc-msys2]
   3. [clang-cl][toc-clang-cl]
4. [Linkers][toc-linkers]
5. [Code coverage][toc-code-coverage]
6. [Compile-time analysis][toc-compile-time-analysis]
   1. [Static analysis][toc-static-analysis]
   2. [Linting][toc-linting]
7. [Run-time analysis][toc-clang-tidy]
8. [Code formatting][toc-run-time-analysis]
9. [Continuous integration][toc-code-formatting]
   1. [Docker images][toc-continuous-integration]
10. [Default third-party libraries][toc-third-party-libraries]

# Package manager

`project_template` uses [Conan][conan-intro] as its [package manager][wiki-pacman]. Conan provides
documentation on [how it can be installed][conan-install].

# Build system

`project_template` uses [CMake][cmake-intro] version at least 3.13 as a [build system][wiki-build-system]
generator. Kitware publish the [latest release][cmake-install] of CMake on their website for
downloading, or you can install the latest version of CMake using Python's package manager, pip:

```bash
python3 -m pip install cmake --user
```

# Compilers and Standard Libraries

The following table documents the supported compilers and the Standard Library implementations that
they support.

<table align="center">
   <tr>
      <th>Compiler</th>
      <th><a href="https://gcc.gnu.org/onlinedocs/libstdc++/faq.html#faq.what">libstdc++-6</a></th>
      <th><a href="https://libcxx.llvm.org/">libc++</a></th>
      <th><a href="https://libcxx.llvm.org/">libc++</a> with MemorySanitizer</th>
   </tr>
   <tr>
      <td align="center">GCC 7</td>
      <td align="center">✔️</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
   <tr>
      <td align="center">GCC 8</td>
      <td align="center">✔️</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
   <tr>
      <td align="center">Clang 7</td>
      <td align="center">✔️</td>
      <td align="center">Requires <a href="https://libcxxabi.llvm.org/">libc++abi</a></td>
      <td align="center">Requires <a href="https://libcxxabi.llvm.org/">libc++abi</a></td>
   </tr>
   <tr>
      <td align="center">Clang 8</td>
      <td align="center">✔️</td>
      <td align="center">Requires <a href="https://libcxxabi.llvm.org/">libc++abi</a></td>
      <td align="center">Requires <a href="https://libcxxabi.llvm.org/">libc++abi</a></td>
   </tr>
   <tr>
      <td align="center">MSVC 2017</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
   <tr>
      <td align="center">MSVC 2019</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
   <tr>
      <td align="center">MSYS2 GCC 7</td>
      <td align="center">✔️</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
   <tr>
      <td align="center">MSYS2 GCC 8</td>
      <td align="center">✔️</td>
      <td align="center">❌</td>
      <td align="center">❌</td>
   </tr>
</table>

Information on how to customise these default profiles (e.g. to support libc++ linking against
libstdc++-6) can be found on the [customising conan profiles page][project-template-conan-profiles].

## MSVC

`project_template` theoretically supports [MSVC] 2017 and MSVC 2019. No testing has been made present.
There is currently a heavy bias toward tools that target GCC and LLVM toolchains. This will
eventually change.

## MSYS2

The 'supported' ABI is libstdc++-6 with [Structured Execption Handling][wiki-seh] and
[POSIX threads][wiki-pthread].

No testing has been made present, and testing this is a lower priority than testing MSVC.

## clang-cl

`project_template` intends to eventually offer support for [clang-cl][llvm-clang-cl]. Integrating
support is an even lower priority than testing MSYS2 at present.

# Linkers

`project_template` turns on [link-time optimisation][llvm-lto] when compiling in release modes by
default. As a result, your toolchain must support LTO in some fashion. When using a Unix-based
system, both [LLD][llvm-lld] and recent versions of [gold][wiki-gold] support LTO. Both supported
versions of MSVC also support LTO.

# Code coverage

`project_template` supports three forms of code coverage:

* [GNU gcov][gnu-gcov] (GCC and LLVM)
* [LLVM SanitizerCoverage][sanitizer-coverage] (LLVM only)
* [LLVM source-based code coverage][llvm-source-based-code-coverage] (LLVM only)

The [`project_template` options page][project-template-options] details how to enable and disable code
coverage.

# Compile-time analysis

## Static analysis

`project_template` does not yet offer support for deep static analysis.

## Linting

### clang-tidy

`project_template` integrates [clang-tidy][llvm-clang-tidy] into the build process. When you compile
your program, clang-tidy is run. If any problems are identified, then the build will fail.

The [`project_template` options page][project-template-options] details how to enable and disable
clang-tidy.

The [clang-tidy checks page][project-template-clang-tidy] documents all of the checks that are enabled
by default.

# Run-time analysis

Assuming that you have the required libraries installed, `project_template` supports many LLVM
sanitisers, including:

* [AddressSanitizer][sanitizer-address]
* [ThreadSanitizer][sanitizer-thread]
* [MemorySanitizer][sanitizer-memory]
* [UndefinedBehaviorSanitizer][sanitizer-undefined]
* [DataFlowSanitizer][sanitizer-data-flow]
* [LeakSanitizer][sanitizer-leak]
* [SanitizerCoverage][sanitizer-coverage]
* [ControlFlowIntegrity][sanitizer-control-flow]
* [SafeStack][sanitizer-safe-stack]
* [ShadowCallStack][sanitizer-shadow-call-stack]

To enable an effective subset of sanitisers, please read the [working with
sanitisers][project-template-sanitisers] page.

# Code formatting

`project_template` provides a `.clang-format` file that can immediately be used with
[clang-format][llvm-clang-format] 8 and later. There is also a pre-push [Git hook][git-hook] that
can prevent pushing to a remote unless the code is formatted according to the prescribed style.

The [clang-format configfuration page][project-template-clang-format] goes into detail about the
default rules, and which rules require specific versions of LLVM.

# Continuous integration

`project_template` uses [Cirrus CI][cirrus-intro] for [continuous integration][wiki-continuous-integration].
Of all of the configurable components of `project_template`, the continuous integration component is
probably both the most important one for you to configure, and also the one that needs the most
changing. It is very unlikely that the current, example configuration is a configuration that suits
your needs.

Please read the [configuring `project_template` continuous integration page][project-template-configure-ci]
for details on how to do this.

## Docker images

Cirrus CI relies upon [Docker][docker-intro] images for Linux and Windows testing. As such,
`project_template` ships with its own Docker images, which have broad support for all of the
aforementioned tools.

`project_template` currently only supports Linux Docker images: this is expected to change in the
immediate future, as lacking Windows Docker images is _the_ blocker for confirming that MSVC and
MSYS2 support is functional, and for adding extended support for Windows development.

# Default third-party libraries

By default, the Conan configuration file will pull and install range-v3, Boost, and doctest. More
information on how to configure this can be found [here][project-template-conanfile].

[project-template-clang-format]:   https://www.cjdb.com.au/404.html
[project-template-clang-tidy]:     https://www.cjdb.com.au/404.html
[project-template-conanfile]:      https://www.cjdb.com.au/404.html
[project-template-conan-profiles]: https://www.cjdb.com.au/404.html
[project-template-configure-ci]:   https://github.com/cjdb/project_template/wiki/Configuring-Continuous-Integration
[project-template-options]:        https://github.com/cjdb/project_template/wiki/CMake-Options
[project-template-sanitisers]:     https://www.cjdb.com.au/404.html

[cirrus-intro]: https://cirrus-ci.org/

[cmake-intro]:   https://cmake.org
[cmake-install]: https://cmake.org/download/

[conan-intro]:   https://docs.conan.io/en/latest/introduction.html
[conan-install]: https://docs.conan.io/en/latest/installation.html

[docker-intro]: https://docs.docker.com/get-started/

[gnu-gcov]: https://gcc.gnu.org/onlinedocs/gcc/Gcov.html

[git-hook]: https://git-scm.com/book/en/v2/Customizing-Git-Git-Hooks

[llvm-clang-cl]:                   https://clang.llvm.org/docs/UsersManual.html#clang-cl
[llvm-clang-format]:               https://clang.llvm.org/docs/ClangFormat.html
[llvm-clang-tidy]:                 https://clang.llvm.org/extra/clang-tidy/
[llvm-lld]:                        https://lld.llvm.org
[llvm-source-based-code-coverage]: https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
[llvm-lto]:                        https://llvm.org/docs/LinkTimeOptimization.html
[llvm-origin]:                     https://clang.llvm.org/docs/MemorySanitizer.html#origin-tracking

[MSYS2]: https://www.msys2.org
[MSVC]: https://visualstudio.microsoft.com

[sanitizer-address]:          https://clang.llvm.org/docs/AddressSanitizer.html
[sanitizer-control-flow]:     https://clang.llvm.org/docs/ControlFlowIntegrity.html
[sanitizer-coverage]:         https://clang.llvm.org/docs/SanitizerCoverage.html
[sanitizer-data-flow]:        https://clang.llvm.org/docs/DataFlowSanitizer.html
[sanitizer-leak]:             https://clang.llvm.org/docs/LeakSanitizer.html
[sanitizer-memory]:           https://clang.llvm.org/docs/MemorySanitizer.html
[sanitizer-safe-stack]:       https://clang.llvm.org/docs/SafeStack.html
[sanitizer-shadow-call-stack]:https://clang.llvm.org/docs/ShadowCallStack.html
[sanitizer-thread]:           https://clang.llvm.org/docs/ThreadSanitizer.html
[sanitizer-undefined]:        https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html

[toc-pacman]:                https://github.com/cjdb/project_template/wiki/Supported-toolchains#package-manager
[toc-build-system]:          https://github.com/cjdb/project_template/wiki/Supported-toolchains#build-system
[toc-compilers]:             https://github.com/cjdb/project_template/wiki/Supported-toolchains#compilers-and-standard-libraries
[toc-msvc]:                  https://github.com/cjdb/project_template/wiki/Supported-toolchains#msvc
[toc-msys2]:                 https://github.com/cjdb/project_template/wiki/Supported-toolchains#msys2
[toc-clang-cl]:              https://github.com/cjdb/project_template/wiki/Supported-toolchains#clang-cl
[toc-linkers]:               https://github.com/cjdb/project_template/wiki/Supported-toolchains#linkers
[toc-code-coverage]:         https://github.com/cjdb/project_template/wiki/Supported-toolchains#code-coverage
[toc-compile-time-analysis]: https://github.com/cjdb/project_template/wiki/Supported-toolchains#compile-time-analysis
[toc-static-analysis]:       https://github.com/cjdb/project_template/wiki/Supported-toolchains#static-analysis
[toc-linting]:               https://github.com/cjdb/project_template/wiki/Supported-toolchains#linting
[toc-clang-tidy]:            https://github.com/cjdb/project_template/wiki/Supported-toolchains#clang-tidy
[toc-run-time-analysis]:     https://github.com/cjdb/project_template/wiki/Supported-toolchains#run-time-analysis
[toc-code-formatting]:       https://github.com/cjdb/project_template/wiki/Supported-toolchains#code-formatting
[toc-continuous-integration]:https://github.com/cjdb/project_template/wiki/Supported-toolchains#continuous-integration
[toc-docker-images]:         https://github.com/cjdb/project_template/wiki/Supported-toolchains#docker-images
[toc-third-party-libraries]: https://github.com/cjdb/project_template/wiki/Supported-toolchains#default-third-party-libraries

[wiki-build-system]:          https://en.wikipedia.org/wiki/Build_automation
[wiki-continuous-integration]:https://en.wikipedia.org/wiki/Continuous_integration
[wiki-gold]:                  https://en.wikipedia.org/wiki/Gold_(linker)
[wiki-pacman]:                https://en.wikipedia.org/wiki/Package_manager
[wiki-pthread]:               https://en.wikipedia.org/wiki/POSIX_Threads
[wiki-seh]:                   https://en.wikipedia.org/wiki/Microsoft-specific_exception_handling_mechanisms#SEH
