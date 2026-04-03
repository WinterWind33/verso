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
const auto major = ver.major(); // 1
const auto minor = ver.minor(); // 0
const auto patch = ver.patch(); // 0

// Setting version numbers
ver.major(2); // Now version is 2.0.0
ver.minor(1); // Now version is 2.1.0
ver.patch(3); // Now version is 2.1.3
```

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

Remember to provide a valid version string in the format `MAJOR.MINOR.PATCH-<pre_release_data>+<build_metadata>` and be sure to respect the semantic versioning rules (see <https://semver.org/#spec-item-2>, so no negative numbers, no non-integer values, leading zeros etc).

If an invalid version string is provided, the `from_string` function will return `std::nullopt`.

### Prerelease data and build metadata

`verso` offers complete support for prerelease data and build metadata as defined in the semantic versioning specification (see <https://semver.org/#spec-item-9> and <https://semver.org/#spec-item-10>).

If you want to use prerelease data and build metadata, you can use the appropriate constructor of the `verso::version` class:

```cpp
#include <verso/verso.hpp>

const verso::version ver1{1, 0, 0, "alpha", "exp.sha.5114f85"}; // Version with prerelease data and build metadata
const verso::version ver2{1, 0, 0, std::nullopt, "21AF26D3----117B344092BD"}; // Version with only build metadata
const verso::version ver3{1, 0, 0}; // Version with no prerelease data and no build metadata

const std::optional prerelease_data = ver1.prerelease_data(); // "alpha"
const std::optional build_metadata = ver1.build_metadata(); // "exp.sha.5114f85"
```

Prerelease data and build metadata are optional, so you can choose to provide them or not when creating a version.

Remember that prerelease data and build metadata are just strings but the set of allowed characters is restricted by the semantic versioning specification (see the above links for more details). If you provide invalid prerelease data or build metadata, the library **will throw a std::invalid_argument exception**, both on construction and on assignment.

If you use one of the many `verso::basic_version` implementations, you are safe from this problem, as they all validate the prerelease data and build metadata according to the specification.

## Compile time support

`verso` is designed and implemented with compile-time support in mind. You can create versions at compile time and perform many operations at compile time as well.

If you want to be sure your code is evaluated at compile time, use static assertions and create `constexpr` versions without having the compiler complaining about it, you can use the `verso::constant_version` alias, which is a `verso::basic_version` that uses `std::uint32_t` for the major, minor, and patch numbers and `std::string_view` for the prerelease data and build metadata, which are all types that can be used in `constexpr` contexts:

```cpp
#include <verso/verso.hpp>

constexpr verso::constant_version ver{1, 0, 0, "alpha", "exp.sha.5114f85"};
```

If you create a constant version with junk data, the compiler will throw a compilation error:

```cpp
#include <verso/verso.hpp>

constexpr verso::constant_version ver{1, 0, 0, "invalid prerelease data!", "...invalid build metadata!"}; // Compilation error: invalid prerelease data and build metadata
```

You can even compare versions at compile time:

```cpp
#include <verso/verso.hpp>

constexpr verso::constant_version ver1{1, 0, 0};
constexpr verso::constant_version ver2{2, 1, 4};
constexpr verso::constant_version ver3{1, 0, 0, "rc.1"};

consteval bool is_ver1_less_than_ver2() {
    return ver1 < ver2; // true
}

consteval bool is_ver1_equal_to_ver3() {
    return ver1 == ver3; // false, because ver3 has prerelease data
}
```

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

### (Experimental) Defining your own version class

Most of the `verso` API is defined in terms of the `verso::Version` concept and functions are all templated to work with any type that satisfies the concept, so you can define your own version class as long as it satisfies the `verso::Version` concept.

In particular, your version class must have the following member functions:

- `major_t`, `minor_t`, and `patch_t` types defined as member types
- `major() -> major_t`, `minor() -> minor_t`, and `patch() -> patch_t` to get the major, minor, and patch numbers

- `prerelease_string_t` and `build_metadata_string_t` types defined as member types
- `prerelease_data() -> std::optional<prerelease_string_t>` and `build_metadata() -> std::optional<build_metadata_string_t>` to get the prerelease data and build metadata

- Your class must be default constructible and constructible with:
  - `Version(major_t major, minor_t minor, patch_t patch)`
  - `Version(major_t major, minor_t minor, patch_t patch, std::optional<prerelease_string_t> prerelease_data)`
  - `Version(major_t major, minor_t minor, patch_t patch, std::optional<prerelease_string_t> prerelease_data, std::optional<build_metadata_string_t> build_metadata)`

- Your class must be equality comparable

This way, your custom version class/struct can be used with all the functions provided by the library, including `to_string` and `from_string`:

```cpp
#include <verso/verso.hpp>

class my_version {
public:
    using major_t = std::uint8_t;
    using minor_t = std::uint16_t;
    using patch_t = std::uint8_t;

    using prerelease_string_t = std::string_view;
    using build_metadata_string_t = std::string_view;

    // Constructors
    my_version() = default;
    my_version(major_t major, minor_t minor, patch_t patch)
        : major_(major), minor_(minor), patch_(patch) {}
    my_version(major_t major, minor_t minor, patch_t patch,
               std::optional<prerelease_string_t> prerelease_data)
        : major_(major), minor_(minor), patch_(patch), prerelease_data_(prerelease_data) {}
    my_version(major_t major, minor_t minor, patch_t patch,
               std::optional<prerelease_string_t> prerelease_data,
               std::optional<build_metadata_string_t> build_metadata)
        : major_(major), minor_(minor), patch_(patch),
          prerelease_data_(prerelease_data), build_metadata_(build_metadata) {}

    // Getters
    major_t major() const { return major_; }
    minor_t minor() const { return minor_; }
    patch_t patch() const { return patch_; }

    std::optional<prerelease_string_t> prerelease_data() const { return prerelease_data_; }
    std::optional<build_metadata_string_t> build_metadata() const { return build_metadata_; }

    bool operator==(const my_version& other) const = default;

private:
    major_t major_{0};
    minor_t minor_{0};
    patch_t patch_{0};

    std::optional<prerelease_string_t> prerelease_data_{std::nullopt};
    std::optional<build_metadata_string_t> build_metadata_{std::nullopt};
};

void test_custom_version() {
    my_version ver{1, 0, 0, "alpha", "exp.sha.5114f85"};

    const std::string ver_str = verso::to_string(ver); // "1.0.0-alpha+exp.sha.5114f85"
    const std::optional<my_version> parsed_ver = verso::from_string<my_version>(ver_str); // parsed_ver has value 1.0.0-alpha+exp.sha.5114f85
}
```

Of course, you should be careful when defining your own version class, as the library functions expect the class to behave in a certain way: for example, most of the library functions do not throw and are marked `noexcept`, so if your class throws exceptions in its member functions, you might get unexpected behavior when using it with the library functions.
