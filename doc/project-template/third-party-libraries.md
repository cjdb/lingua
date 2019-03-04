# Third-party libraries (WIP)

An important part of `project_template` is to offer support for easily adding third-party libraries to
your project.

# Default third-party libraries

By default, `project_template` supports:

* [doctest][github-doctest] (v2.2.0)
* [range-v3][github-range-v3] (v1.0-beta)

# Adding third-party libraries

Adding new libraries to your project is a short process.

1. Find your package. Many packages can be found in the [Bintray Conan Centre][bintray-conan].
2. If it isn't already present, add the package's remote to
   [`config/conan/remotes.txt`][project-template-remotes] in the project.
   1. You will need to provide a remote name and the remote's URL.
   2. On Bintray, you can get this by pressing the package's _Set Me Up!_ button.
3. Add the package's reference to the list of packages assigned to the `requires` member.
4. Add the remote to Conan by running `conan remote add <remote-name> <remote-url>`.
4. Run `conan install <package-reference> --build missing --remote <remote-name>`.
5. Run `python3 find_cmake_package_name.py --package=<package-reference>`.
   1. If you do not have a `.conan` directory in your home directory (user directory on Windows),
      you'll also need to provide the flag `--conan-root=/path/to/.conan`.
6. Add `find_package(<output-from-find_cmake_package_name.py> REQUIRED)` to
   [`cmake/project-template-packages.cmake`][project-template-packages].

## Example

Suppose we'd like to use [Hana Dusíková's compile-time regular expression][bintray-ctre] library in
our project.

1. After clicking on _Set Me Up!_, we see that the required remote is _not_ in our `remotes.txt`,
   and so we add it, so that others can conveniently start using our package if they clone our repo.
2. We run `conan remote add ctre https://api.bintray.com/conan/hanickadot/ctre` so that the remote
   is recognised locally.
3. Next, we'll run `conan install CTRE/v2.5.1@ctre/stable --build missing --remote ctre`.
4. (TODO)

# Removing third-party libraries

Your project mightn't need to use one or more of the default libraries. Keeping them there doesn't
hurt anyone (and makes them trivially accessible if you choose to use them later on). It does,
however, potentially increase the time required to run `conan install`. This, in turn, will affect
your continuous integration's round-trip time.

To remove a package, you can follow this process.

1. Remove the call to `find_package(package` from `cmake/project-template-packages.cmake`.
2. Optionally run `conan remove <package-reference>`.
3. Optionally run `conan remote remove <remote-name>`.
4. Remove the package reference from the list of packages assigned to the `requires` member.
5. Remove `<remote-name>` from `config/conan/remotes.txt`.

[project-template-packages]: https://github.com/cjdb/project_template/blob/master/cmake/packages/project-template-packages.cmake
[project-template-remotes]: https://github.com/cjdb/project_template/blob/master/config/conan/remotes.txt

[bintray-conan]: https://bintray.com/conan/conan-center
[bintray-ctre]: https://bintray.com/hanickadot/ctre/CTRE%3Actre

[github-doctest]: https://github.com/onqtam/doctest
[github-range-v3]: https://github.com/ericniebler/range-v3/tree/v1.0-beta
