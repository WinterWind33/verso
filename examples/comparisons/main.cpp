// Copyright (c) 2026 Andrea Ballestrazzi

// C++ STL
#include <iostream>
#include <string>

#include <verso/verso.hpp>

std::string strong_ordering_to_string(const std::strong_ordering& ordering) {
    if (ordering < 0) {
        return "less";
    } else if (ordering > 0) {
        return "greater";
    } else {
        return "equal";
    }
}

void print_comparison_result(const verso::version& version1, const verso::version& version2) {
    std::cout << "\nComparing version1={" << verso::to_string(version1) << "} and version2={"
              << verso::to_string(version2) << "}:" << std::endl;
    std::cout << "version1 < version2: " << (version1 < version2) << std::endl;
    std::cout << "version1 > version2: " << (version1 > version2) << std::endl;
    std::cout << "version1 == version2: " << (version1 == version2) << std::endl;
    std::cout << "version1 != version2: " << (version1 != version2) << std::endl;
    std::cout << "version1 <= version2: " << (version1 <= version2) << std::endl;
    std::cout << "version1 >= version2: " << (version1 >= version2) << std::endl;

    // Operator <=> is also supported, and it returns a std::strong_ordering value that can be
    // compared to zero to determine the ordering of the versions.
    const auto comparison_result = version1 <=> version2;
    std::cout << "version1 <=> version2: " << strong_ordering_to_string(comparison_result)
              << std::endl;
}

int main() {
    std::cout << std::boolalpha;

    // Core versions
    {
        const verso::version version1{2, 1, 4};
        const verso::version version2{2, 1, 5};
        const verso::version version3{2, 4, 0};
        print_comparison_result(version1, version2);
        print_comparison_result(version1, version3);
    }

    // Prereleases
    {
        const verso::version version1{2, 1, 4, "alpha"};
        const verso::version version2{2, 1, 4, "beta"};
        const verso::version version3{2, 1, 4, "alpha.1"};
        print_comparison_result(version1, version2);
        print_comparison_result(version1, version3);
    }

    // Build metadata is ignored in comparisons, so these versions are considered equal.
    {
        const verso::version version1{2, 1, 4, std::nullopt, "build.1"};
        const verso::version version2{2, 1, 4, std::nullopt, "build.2"};
        print_comparison_result(version1, version2);

        const verso::version version3{2, 1, 4, "alpha", "build.1"};
        print_comparison_result(version1, version3);
    }

    return 0;
}
