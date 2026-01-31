# verso - A simple C++ semantic versioning library

**verso** is a simple header-only C++ library for supporting semantic versioning in C++ projects. The library is written and requires C++ 20, so make sure your compiler supports it.

The library has one header file, `verso/verso.hpp`, just include it in your project and you're ready to go. The library does not have any third-party dependencies.

Enjoy!

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
# You need to have CMake 3.16+
add_subdirectory(path/to/verso)
target_link_libraries(your_target PRIVATE verso)
```

And you're ready to go!

## Getting started with verso

You can find a getting started guide in the [docs/getting-started.md](docs/getting-started.md) file.

## Development documentation

Make sure to read the [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) file for guidelines on contributing to the project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
