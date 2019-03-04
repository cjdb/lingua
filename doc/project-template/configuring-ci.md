# Configuring Continuous Integration

# Pre-requisite reading

Before reading this page, you should probably read how to [configure the build system
generator][project-template-configure-cmake].

# Setting up Cirrus CI on GitHub

In order to use Cirrus CI, you'll need to provide the app with access to your project. Please
consult both the [GitHub docs for personal acounts][github-apps-personal], the [GitHub docs for
organisations][github-apps-org] and the [Cirrus CI][cirrus-docs] docs on how to do
this.

# Understanding the example Continuous Integration

The example `.cirrus.yml` has been crafted to help you explore the available configuration options,
and to check that certain supported toolchains work. You will _very probably_ want to change this
configuration so that it only compiles against the toolchains that you wish to support.

This will have three net positives:

1. There will be less strain on the Cirrus CI hardware.
2. Your CI jobs will finish a lot faster if there are fewer jobs to complete.
3. You'll be able to use more resources if there are fewer jobs to complete (as there will be less
   strain on the hardware).

## Platforms

### Ubuntu 18.04

The example continuous integration script is written to support Linux through the
`cjdb/cirrus-cpp:bionic` Docker image. That image supports all of the non-Windows toolchains that
are listed in the supported toolchains list.

The default `project_template` configuration file uses four CPUs and 8 GB memory on this platform.

## Jobs

There are nineteen default jobs. One for each version of GCC from versions 5 -- 8, and three for
each version of LLVM from versions 4 -- 8 (one for each distinct ABI).

### Debug and Release builds

Debug and Release builds are executed linearly on the same job. This is because having separate jobs
for Debug and Release would double the number of jobs required for completion, which places way too
much strain on the hardware. If your project does not require all of the toolchains that are listed
in the default configuration, you may like to add it. More on that is below.

## Scripts

Due to the fact that the Debug and Release builds needed to be streamlined, and because it can be
convenient to run something that resembles continuous integration locally, `project_template` ships
with scripts that automate the entire build process, from start to finish. These scripts can be
found in the top-level path `config/scripts`.

# Customising the configuration file

The best way to learn how to customise a Cirrus CI config file is to experiment. Play with the file
and see what works.

## Environment

The environment is comprised of variables and matrices of variables.

```yaml
# Example 1
task:
  env:
    PROJECT_NAME: hello
    matrix:
      - PROFILE: gcc-5
      - PROFILE: gcc-6
      - PROFILE: gcc-7
      - PROFILE: gcc-8
```

Example 1 demonstrates a project that has four configurations. Each configuration has an environment
variable called `PROJECT_NAME`, which has the value `hello`, and an environment variable `PROFILE`,
which is different value for each configuration. The Cirrus back-end will create four instances of
the project, each with a different value for `PROFILE`. You'd presumably do something with the
`PROFILE` variable.

If you have two values on the same level, then they are multiplied.

```yaml
# Example 2
task:
  env:
    PROJECT_NAME: hello
    matrix:
      - PROFILE: gcc-5
      - PROFILE: gcc-6
      - PROFILE: gcc-7
      - PROFILE: gcc-8
    matrix:
      - BUILD_TYPE: Debug
      - BUILD_TYPE: Release
      - BUILD_TYPE: MinSizeRel
      - BUILD_TYPE: RelWithDebInfo
```

In this case, there are _sixteen_ different configurations: each configuration has a `PROJECT_NAME`,
a `PROFILE`, and a `BUILD_TYPE`; but for every `PROFILE`, we need to associate it with a `BUILD_TYPE`.

```yaml
# Example 3
task:
  env:
    PROJECT_NAME: hello
    REQUIRED_SANITIZERS: Address;Undefined
    matrix:
      - PROFILE: gcc-8
        CODE_COVERAGE: gcov
      - PROFILE: clang-8
        CODE_COVERAGE: LLVMSourceCoverage
        OPTIONAL_SANITIZERS: ControlFlowIntegrity
```

Example 3 demonstrates having multiple values per matrix entry. When `PROFILE` is `gcc-8`,
`CODE_COVERAGE` will be `gcov`, and `OPTIONAL_SANITIZERS` will be the empty string. When `PROFILE`
is `clang-8`, `CODE_COVERAGE` will be `LLVMSourceCoverage`, and `OPTIONAL_SANITIZERS` will be
`ControlFlowIntegrity`.

Finally, we can also nest matrices. Only values on the same level are multiplied.

```yaml
# Example 4
task:
  env:
    PROJECT_NAME: hello
    matrix:
      - PROFILE: gcc-5
      - PROFILE: gcc-6
      - PROFILE: gcc-7
      - PROFILE: gcc-8
        matrix:
          - BUILD_TYPE: Debug
          - BUILD_TYPE: Release
          - BUILD_TYPE: MinSizeRel
          - BUILD_TYPE: RelWithDebInfo
```

Here, only the `gcc-8` profile has a `BUILD_TYPE`: everyone else does not. If you don't define a
variable, then its value is the empty string when read.

## Scripts

There are currently six scripts:

* `config/hooks/clang-format.sh`
* `config/scripts/pre-install.sh`
* `config/scripts/conan-install.sh`
* `config/scripts/build-configure.sh`
* `config/scripts/build.sh`
* `config/scripts/test.sh`

`clang-format.sh` is used to ensure that all source files conform to the style guide made by
clang-format. If the script notes any changes to files made by clang-format or otherwise (such as
changing permissions), the job will fail.

Due to the stress that was being placed on the Cirrus hardware when 38 instances were spun up, the
scripts in the `config/scripts/` path were created to minimise the amount of repetition in
`.cirrus.yml`. Each script is run exactly once per build type.

Each script is run like a sub-job. When a script completes, the next script starts. If a script
fails, then an error is reported, and the job is stopped. Each sub-job must have `_script` as a
suffix. If you choose to add a `BUILD_TYPE` matrix, you can eliminate the need for repeated
invocations of the script.

### pre-install.sh

Pre-installation (1) creates a directory named `build-${1}`, where `${1}` is the first argument
passed to the script; and (2) also installs all of your Conan configuration, such as your remotes
and profiles.

Later scripts assume that `${1}` is one of `Debug`, `Release`, `MinSizeRel`, or `RelWithDebInfo`.

```bash
> # Example invocation
> export BUILD_TYPE=Debug
> config/scripts/pre-install.sh ${BUILD_TYPE}
> ls
build-Debug
>
```

### conan-install.sh

Given a build type and Conan profile name as parameters, installs all necessary libraries and
for your project.

```bash
> # Example invocation, carried on from pre-install.sh
> export PROFILE=gcc-8
> config/scripts/conan-install.sh ${BUILD_TYPE} ${PROFILE}
```

### build-configure.sh

Generates the build system, given the:

1. project name
2. build type
3. desired code coverage
4. required sanitisers
5. optional sanitisers

```bash
> # Example invocation, carried on from conan-install.sh
> export PROJECT_NAME=hello
> export CODE_COVERAGE=gcov
> export REQUIRED_SANITIZERS=Address;Undefined
> export OPTIONAL_SANITIZERS=
> config/scripts/build-configure.sh ${PROJECT_NAME} \
   ${BUILD_TYPE}                                    \
   ${CODE_COVERAGE}                                 \
   ${REQUIRED_SANITIZERS}                           \
   ${OPTIONAL_SANITIZERS}
```

### build.sh

Builds the project. Takes the build type as a parameter.

```bash
> # Example invocation, carried on from build-configure.sh
> config/scripts/build.sh ${BUILD_TYPE}
```

### test.sh

Runs the project tests. Takes the build type as a parameter.

```bash
> # Example invocation, carried on from build-configure.sh
> config/scripts/test.sh ${BUILD_TYPE}
```

[project-template-configure-cmake]: https://github.com/cjdb/project_template/wiki/CMake-Options
[cirrus-docs]: https://cirrus-ci.org/
[github-apps-personal]: https://help.github.com/en/articles/installing-an-app-in-your-personal-account
[github-apps-org]: https://help.github.com/en/articles/installing-an-app-in-your-organization
