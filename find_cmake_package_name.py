#!/usr/bin/python
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
import argparse
import crayons
import pystache
import re
import sys
import subprocess

from os import environ, walk, path


def parse_args():
    """Parses the program's arguments."""

    if not environ.get("HOME"):
        raise RuntimeError("Unable to get the HOME directory of the user.")

    parser = argparse.ArgumentParser(
        description="Find a package's CMake name.")

    parser.add_argument(
        '--package',
        metavar='package',
        nargs=1,
        help='The name of the package Conan has installed.')
    parser.add_argument(
        '--conan-root',
        metavar='conan_root',
        nargs=1,
        default=f'{environ["HOME"]}/.conan/data',
        help='The directory to search for the package name.')
    args = parser.parse_args()
    return args.package[0], args.conan_root


def split_package_ref(package_ref):
    """Split the package reference into its name, version, repository, and branch.

    package_ref -- The Conan package reference to split.
    """
    match = re.match(
        r"""([^/]+)/  # package name
                         ([^@]+)@  # version
                         ([^/]+)/  # repository
                         (.+)      # branch name""", package_ref, re.VERBOSE)

    if match:
        package_name = match.group(1)
        version = match.group(2)
        repository = match.group(3)
        branch = match.group(4)
    else:
        raise RuntimeError(f'Package reference "{package_name}" is invalid.')

    return package_name, version, repository, branch


def find_path(conan_root, package_info):
    """Confirm that the path to the exists.

    conan_root -- The root directory for the Conan metadata.

    package_info -- The split details of the package reference.
    """

    package_name, version, repository, branch = package_info
    package_path = f"{conan_root}/{package_name}/{version}/{repository}/{branch}"

    if not path.isdir(package_path):
        print(
            f'Error: path name "{package_path}" is not a directory.',
            file=sys.stderr)
        exit(1)
    return package_path


config_suffix = re.compile('(C|-c)onfig.cmake$')


def find_configuration(package_path):
    """Find CMake configuration files.

    package_path -- The directory path to start searching from.
    """

    configurations = list()

    for root, _, files in walk(package_path):
        configurations += filter(lambda file: config_suffix.search(file),
                                 files)

    configurations = list(
        map(lambda i: config_suffix.sub('', i), dict.fromkeys(configurations)))

    if len(configurations) == 0:
        raise RuntimeError("Could not find configuration")

    return configurations[0]


target_suffix = re.compile('Targets.cmake$')


def find_linkage(package_path, package_name):
    """Find CMake configuration files.

    package_path -- The directory path to start searching from.
    package_name -- The name of the package
    """

    for root, _, files in walk(package_path):
        for cmake_file in filter(
                lambda f: config_suffix.search(f) or target_suffix.search(f),
                files):
            with open(f"{root}/{cmake_file}") as to_read:
                data = to_read.read()
                if "::" in data:
                    result = re.search("([A-Za-z0-9_-]+)::[A-Za-z0-9_-]+",
                                       data)
                    if result[1] == package_name:
                        return result[0]
    return package_name


def print_template(template, substitutions):
    """
        Prints out a string template using a string template engine. Does not write a new line at
        the end of the write.

        template -- string containing the string template
        substitutions -- a mapping for substituting template parameters
    """
    print(pystache.render(template, substitutions), end='')


def print_title(title):
    """
        Prints out the title of a section of output. The title is printed in the colour cyan.

        title -- The title of the section.
    """
    print(f"--------------- {crayons.cyan(title)} ---------------")


def how_to_find_package(library):
    """
        Documents how to find a package.

        library -- The name of the package to find.
    """
    print_title("Importing a package")

    find_package = crayons.green(f'find_package({library} REQUIRED)')
    packages_file = crayons.green('cmake/project_target-packages.cmake')
    print(f"Add `{find_package}` to {packages_file}.\n")


def how_to_use_library(library):
    """
        Documents how to link a library to a target.

        library -- The name of the package to link.
    """

    template = ""
    with open("config/python/link.st") as f:
        template = f.read()

    link_type_key = "link_type"
    library_key = "library"
    verb_key = "verb"
    recommendation_key = "recommendation"

    print_title("Linking your package")
    print("Add exactly one of:")

    # Private linkage
    substitutions = {
        link_type_key: crayons.green("LIBRARIES"),
        library_key: crayons.green(library),
        verb_key: "linking against the target only",
        recommendation_key: crayons.green("recommended")
    }
    print_template(template, substitutions)

    # Interface linkage
    substitutions[link_type_key] = crayons.yellow("INTERFACE_LIBRARIES")
    substitutions[library_key] = crayons.yellow(library)
    substitutions[verb_key] = "forward linking only"
    substitutions[recommendation_key] = crayons.yellow("only if necessary")
    print_template(template, substitutions)

    # Public linkage
    substitutions[link_type_key] = crayons.yellow("PUBLIC_LIBRARIES")
    substitutions[
        verb_key] = "both linking against the target and forward linking"
    print_template(template, substitutions)


def main():
    try:
        package_ref, conan_root = parse_args()
        package_path = find_path(conan_root, split_package_ref(package_ref))

        package_name = find_configuration(package_path)
        package_link = find_linkage(package_path, package_name)

        how_to_find_package(package_name)
        how_to_use_library(package_link)
    except RuntimeError as error:
        print(f"{error} for {package_ref}.")


if __name__ == "__main__":
    main()
