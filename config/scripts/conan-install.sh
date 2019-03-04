#!/bin/bash
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
BUILD_TYPE=${1}
PROFILE=${2}
CODE_COVERAGE=${3}
REQUIRED_SANITIZERS=${4}
OPTIONAL_SANITIZERS=${5}
SANITIZE_RELEASE=${6}
ENABLE_CLANG_TIDY=${7}
CLANG_TIDY_PATH=${8}

export CONAN_USER_HOME="${CIRRUS_WORKING_DIR}"

mkdir -p build-${BUILD_TYPE}                              && \
cd build-${BUILD_TYPE}                                    && \
conan install ..                                        \
   --build                                              \
   --profile=${PROFILE}                                 \
   --settings build_type=${BUILD_TYPE}                  \
   --options code_coverage=${CODE_COVERAGE}             \
   --options required_sanitizers=${REQUIRED_SANITIZERS} \
   --options optional_sanitizers=${OPTIONAL_SANITIZERS} \
   --options sanitize_release=${SANITIZE_RELEASE}       \
   --options enable_clang_tidy=${ENABLE_CLANG_TIDY}     \
   --options clang_tidy_path=${CLANG_TIDY_PATH}         \
