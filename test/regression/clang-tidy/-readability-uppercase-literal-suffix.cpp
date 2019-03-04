//
//  Copyright Christopher Di Bella
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

/// This test checks that readability-uppercase-literal-suffix is *disabled*.
/// \see https://clang.llvm.org/extra/clang-tidy/checks/readability-uppercase-literal-suffix.html.

void x()
{
   auto x = 0l;
   (void)x;

   auto y = 0.0f;
   (void)y;
}
