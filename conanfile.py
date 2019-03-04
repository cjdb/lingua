#
#  Copyright Morris Hafner
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
from conans import ConanFile, CMake, RunEnvironment, tools
import os


class Lingua(ConanFile):
    name = "lingua"
    description = "A project-starter for C++ projects."
    author = "Christopher Di Bella"
    license = "Apache License, Version 2.0"
    url = "https://github.com/cjdb/lingua.git"
    version = "0.1"
    topics = ("cpp", "c++", "cmake", "conan")
    settings = ("os", "compiler", "arch", "build_type")
    generators = ("cmake", "cmake_paths", "virtualrunenv")
    options = {
        "code_coverage": ["Off", "gcov", "LLVMSourceCoverage"],
        "required_sanitizers": "ANY",
        "optional_sanitizers": "ANY",
        "sanitize_release": ["Off", "On"],
        "enable_clang_tidy": ["Off", "On"],
        "clang_tidy_path": "ANY"
    }
    default_options = {
        "code_coverage": "Off",
        "required_sanitizers": "",
        "optional_sanitizers": "Address;Undefined;ControlFlowIntegrity",
        "sanitize_release": "Off",
        "enable_clang_tidy": "On",
        "clang_tidy_path": "/usr/bin/clang-tidy"
    }
    requires = ("cjdb/0.1@cjdb/beta",
                "doctest/2.2.0@bincrafters/stable",
                "expected/master@cjdb/stable",
                "fmt/5.3.0@bincrafters/stable",
                "range-v3/v1.0-beta@cjdb/beta")
    exports_sources = (".clang*", "cmake/*", "CMakeLists.txt", "include/*",
                       "source/*", "test/*", "LICENSE.md")
    build_policy = "always"
    no_copy_source = True

    def define_from_options(self, cmake):
        cmake.definitions[f"{self.name}_CODE_COVERAGE"] = self.options.code_coverage
        cmake.definitions[f"{self.name}_REQUIRED_SANITIZERS"] = self.options.required_sanitizers
        cmake.definitions[f"{self.name}_OPTIONAL_SANITIZERS"] = self.options.optional_sanitizers
        cmake.definitions[f"{self.name}_SANITIZE_RELEASE"] = self.options.sanitize_release
        cmake.definitions[f"{self.name}_ENABLE_CLANG_TIDY"] = self.options.enable_clang_tidy
        cmake.definitions[f"{self.name}_CLANG_TIDY_PATH"] = self.options.clang_tidy_path
        return cmake

    def build(self):
        cmake = CMake(self)

        cmake = self.define_from_options(cmake)
        cmake.configure()
        cmake.build()

        env_build = RunEnvironment(self)
        with tools.environment_append(env_build.vars):
            cmake.test(output_on_failure=True)

    def package(self):
        cmake = CMake(self)

        cmake.install()
        self.copy(
            "LICENSE.md", dst="licenses", ignore_case=True, keep_path=False)

    def package_info(self):
        self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))
