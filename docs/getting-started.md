# Getting started

Getting started with Verso is easy. After you configured your environment, either by cloning the repository, adding it as a submodule, or downloading the header file, you can start using it in your C++ projects.

## Basic Usage

Include the Verso header file in your C++ source code:

```cpp
#include <verso/verso.hpp>
```

After that, you can freely use classes and function you can find in the `verso` namespace.

```cpp

// ...
verso::version ver{1, 0, 0}; // Creates a version 1.0.0

// Using version numbers
const auto major = ver.major; // 1
const auto minor = ver.minor; // 0
const auto patch = ver.patch; // 0

// Setting version numbers
ver.major = 2; // Now version is 2.0.0
ver.minor = 1; // Now version is 2.1.0
ver.patch = 3; // Now version is 2.1.3
```

It really is just a simple structure.

You can fully compare versions using the standard comparison operators (even the three-way comparison operator if your compiler supports it):

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
auto cmp = (ver1 <=> ver2); // std::strong_ordering::less

```

### String conversions

You can convert versions to and from strings using the `to_string` function and the `from_string` function:

To string:

```cpp
#include <verso/verso.hpp>

const verso::version ver{1, 2, 3};
const std::string ver_str = verso::to_string(ver); // "1.2.3"
```

From string:

```cpp
#include <verso/verso.hpp>

const std::string ver_str = "1.2.3";
const std::optional<verso::version>ver = verso::from_string<verso::version>(ver_str); // ver has value 1.2.3

const std::string invalid_ver_str = "1.2"; // Invalid version string
const std::optional<verso::version> invalid_ver = verso::from_string<verso::version>(invalid_ver_str); // invalid_ver has no value, std::nullopt
```

Remember to provide a valid version string in the format `MAJOR.MINOR.PATCH` and be sure to respect the semantic versioning rules (see <https://semver.org/#spec-item-2>, so no negative numbers, no non-integer values, leading zeros etc).

If an invalid version string is provided, the `from_string` function will return `std::nullopt`.

## Advanced Usage

You can almost completely customize the types used by the library. By default, `verso::version` uses `std::uint32_t` for the major, minor, and patch numbers, but you can change that by providing your own types.

The library defines the templated class `verso::basic_version` which takes only one template parameter, the type traits structure which defines the types used by the class.

### Available fully specialized aliases

By default, the library provides five fully specialized aliases for `verso::basic_version`:

- `verso::version` - uses `std::uint32_t` for major, minor, and patch
- `verso::version8` - uses `std::uint8_t` for major, minor, and patch
- `verso::version16` - uses `std::uint16_t` for major, minor, and patch
- `verso::version32` - uses `std::uint32_t` for major, minor, and patch
- `verso::version64` - uses `std::uint64_t` for major, minor, and patch

#### Default verso types

By default, the library uses `std::uint32_t` for the major, minor, and patch numbers in the `verso::version` alias.

### Other available aliases

The library also provides other helper aliases for `verso::basic_version`:

- `verso::uniform_version<T>` - uses the same type `T` for major, minor, and patch

So, for example, if you decide to use `verso::uniform_version<std::uint_fast32_t>`, the major, minor, and patch numbers will all be of type `std::uint_fast32_t`:

```cpp
using my_version = verso::uniform_version<std::uint_fast32_t>;
my_version ver{1, 0, 0}; // Creates a version 1.0.0 with std::uint_fast32_t types
```

### Customizing types

#### Types restrictions

Remember that, as per semantic versioning rules (see <https://semver.org/#spec-item-2>), the major, minor, and patch numbers must be non-negative integers. The library restricts the types used for these numbers to unsigned integer types only by defining C++ concepts to enforce this constraint.

Trying to use a signed integer type or a non-integer type will result in a compilation error.

#### Defining your own type traits structure

As explained before, you can define your own type traits structure to customize the types used by `verso::basic_version`.

You can either use the provided `verso::version_traits<MajorType, MinorType, PatchType>` template or define your own structure. The structure must abide to the `verso::VersionTraits` concept, so make sure to add a `static_assert` to check that.

An example using the provided `verso::version_traits` template:

```cpp
#include <verso/verso.hpp>

// Define your own type traits structure
using my_version_traits = verso::version_traits<std::uint16_t, std::uint32_t, std::uint64_t>;
// Ensure it satisfies the VersionTraits concept
static_assert(verso::VersionTraits<my_version_traits>);

// Define your own version type using the custom traits
// In this case your version will use std::uint16_t for major,
// std::uint32_t for minor, and std::uint64_t for patch
using my_version = verso::basic_version<my_version_traits>;
my_version ver{1, 0, 0}; // Creates a version 1.0.0 with custom types
```

An example defining your own type traits structure:

```cpp
#include <verso/verso.hpp>

// Define your own type traits structure
struct my_version_traits {
    using major_t = std::uint8_t; // Major version type
    using minor_t = std::uint16_t; // Minor version type
    using patch_t = std::uint8_t; // Patch version type
};
// Ensure it satisfies the VersionTraits concept
static_assert(verso::VersionTraits<my_version_traits>);

// Define your own version type using the custom traits
// In this case your version will use std::uint8_t for major,
// std::uint16_t for minor, and std::uint8_t for patch
using my_version = verso::basic_version<my_version_traits>;
my_version ver{1, 0, 0}; // Creates a version 1.0.0 with custom types
```
