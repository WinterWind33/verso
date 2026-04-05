// Copyright (c) 2026 Andrea Ballestrazzi

// C++ STL
#include <iostream>

#include <verso/verso.hpp>

#include "version.hpp"

int main() {
    std::cout << "Generated version: " << verso::to_string(example::GENERATED_VERSION) << std::endl;
    return 0;
}
