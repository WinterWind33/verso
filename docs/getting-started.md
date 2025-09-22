# Getting started

Getting started with Verso is easy. After you configured your environment, either by cloning the repository, adding it as a submodule, or downloading the header file, you can start using it in your C++ projects.

## Basic Usage

Include the Verso header file in your C++ source code:

```cpp
#include <verso/verso.h>
```

After that, you can freely use classes and function you can find in the `verso` namespace.

```cpp

// ...
verso::version ver{1, 0, 0}; // Creates a version 1.0.0

// Using version numbers
const auto major = ver.major(); // 1
const auto minor = ver.minor(); // 0
const auto patch = ver.patch(); // 0

// Setting version numbers
ver.major(2); // Now version is 2.0.0
ver.minor(1); // Now version is 2.1.0
ver.patch(3); // Now version is 2.1.3
```

You can fully compare versions using the standard comparison operators:

```cpp
// Comparing versions
const verso::version ver1{1, 0, 0};
const verso::version ver2{2, 1, 4};

bool isEqual = (ver1 == ver2); // false
bool isNotEqual = (ver1 != ver2); // true
bool isLess = (ver1 < ver2); // true
bool isLessEqual = (ver1 <= ver2); // true
bool isGreater = (ver1 > ver2); // false
bool isGreaterEqual = (ver1 >= ver2); // false

```
