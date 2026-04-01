# verso - A simple C++ semantic versioning library

**verso** is a simple header-only C++ library for supporting semantic versioning in C++ projects. The library is written and requires C++ 20, so make sure your compiler supports it.

The library has one header file, `verso/verso.hpp`, just include it in your project and you're ready to go. The library does not have any third-party dependencies.

Enjoy!

## Build status

[![Linux x64 GCC Build](https://github.com/WinterWind33/verso/actions/workflows/linux-x64-gcc.yml/badge.svg)](https://github.com/WinterWind33/verso/actions/workflows/linux-x64-gcc.yml) [![Windows x64 MSVC Build](https://github.com/WinterWind33/verso/actions/workflows/windows-x64-msvc.yml/badge.svg)](https://github.com/WinterWind33/verso/actions/workflows/windows-x64-msvc.yml) [![MacOS x64 Clang Build](https://github.com/WinterWind33/verso/actions/workflows/macos-x64-clang.yml/badge.svg)](https://github.com/WinterWind33/verso/actions/workflows/macos-x64-clang.yml)

## Getting verso

You can get verso by cloning this repository, making it a submodule in your project, or downloading the single header file.

### Cloning the repository

You can clone the repository:

```bash
git clone https://github.com/WinterWind33/verso.git
cd verso
```

### As a project submodule

If you want to include verso as a submodule in your project:

```bash
git submodule add https://github.com/WinterWind33/verso.git
git submodule update --init --recursive
```

After this, you can use CMake to include verso in your project:

```cmake
# You need to have CMake 3.19+
add_subdirectory(path/to/verso)
target_link_libraries(your_target PRIVATE verso)
```

And you're ready to go!

## Getting started with verso

To get started with verso, just include the header file in your C++ source code:

```cpp
#include <verso/verso.hpp>
```

And then you can start using the classes and functions in the `verso` namespace:

```cpp
verso::version ver{1, 2, 3}; // Creates a version 1.2.3

// Setting version numbers
ver.major = 2; // Now version is 2.2.3
ver.minor = 1; // Now version is 2.1.3
ver.patch = 3; // Now version is 2.1.3
std::cout << "Version: " << verso::to_string(ver) << '\n'; // Version: 2.1.3
```

Structured bindings:

```cpp
// Structured binding
auto [major, minor, patch] = ver; // major = 2, minor = 1, patch = 3
```

Getting components as a tuple:

```cpp
// As a std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>
const auto version_tuple = verso::components(ver); // std::tuple{2, 1, 3}
const auto major = std::get<0>(version_tuple); // 2
const auto minor = std::get<1>(version_tuple); // 1
const auto patch = std::get<2>(version_tuple); // 3
```

Parsing versions from strings:

```cpp
std::optional<verso::version> parsed_ver = verso::from_string<verso::version>("1.43.5");
assert(parsed_ver.has_value());

const auto major = parsed_ver->major; // 1
const auto minor = parsed_ver->minor; // 43
const auto patch = parsed_ver->patch; // 5
```

You can find the full getting started guide in the [docs/getting-started.md](docs/getting-started.md) file.

## Development documentation

Make sure to read the [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) file for guidelines on contributing to the project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
