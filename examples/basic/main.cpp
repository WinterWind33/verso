// Copyright (c) 2025-2026 Andrea Ballestrazzi

// C++ STL
#include <iostream>

#include <verso/verso.hpp>

// In this example, we demonstrate the basic usage of the verso library.
// You can create a simple version object by providing the major, minor and patch version numbers.
// You can print the version numbers using the major(), minor() and patch() member functions, and
// you can convert the version to a string using the to_string() function.
int main() {
    const verso::version basicVersion{1, 0, 0};

    std::cout << "Major version: " << basicVersion.major() << std::endl;
    std::cout << "Minor version: " << basicVersion.minor() << std::endl;
    std::cout << "Patch version: " << basicVersion.patch() << std::endl;

    std::cout << "Basic version: " << verso::to_string(basicVersion) << std::endl;

    // Display the version of the library itself, which is defined in the verso.hpp header file as a
    // constant_version object.
    std::cout << "verso version: " << verso::to_string(verso::verso_version) << std::endl;

    return 0;
}
