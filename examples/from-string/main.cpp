// Copyright (c) 2026 Andrea Ballestrazzi

// C++ STL
#include <cassert>
#include <iostream>
#include <string>

#include <verso/verso.hpp>

int main() {
    // Happy paths
    const auto version1 = verso::from_string<verso::version>("2.1.4");
    const auto version2 = verso::from_string<verso::version>("2.1.4-alpha.1+build.123");
    const auto version3 = verso::from_string<verso::version>("2.1.4-beta+build.456");
    assert(version1.has_value());
    assert(version2.has_value());
    assert(version3.has_value());
    std::cout << "version1: " << verso::to_string(*version1) << std::endl;
    std::cout << "version2: " << verso::to_string(*version2) << std::endl;
    std::cout << "version3: " << verso::to_string(*version3) << std::endl;

    {
        const std::string version1Str{"2.1"};
        const std::string version2Str{"2.1.0.3.4"};
        const std::string version3Str{"2.1.4-000002"};
        const std::string version4Str{"2.1.4+build..123"};

        // Invalid version strings
        const auto invalidVersion1 = verso::from_string<verso::version>(version1Str);
        const auto invalidVersion2 = verso::from_string<verso::version>(version2Str);
        const auto invalidVersion3 = verso::from_string<verso::version>(version3Str);
        const auto invalidVersion4 = verso::from_string<verso::version>(version4Str);
        assert(!invalidVersion1.has_value());
        assert(!invalidVersion2.has_value());
        assert(!invalidVersion3.has_value());
        assert(!invalidVersion4.has_value());

        std::cout << "Invalid version strings:" << std::endl;
        std::cout << "version1Str: " << version1Str << " -> "
                  << (invalidVersion1.has_value() ? "valid" : "invalid") << std::endl;
        std::cout << "version2Str: " << version2Str << " -> "
                  << (invalidVersion2.has_value() ? "valid" : "invalid") << std::endl;
        std::cout << "version3Str: " << version3Str << " -> "
                  << (invalidVersion3.has_value() ? "valid" : "invalid") << std::endl;
        std::cout << "version4Str: " << version4Str << " -> "
                  << (invalidVersion4.has_value() ? "valid" : "invalid") << std::endl;
    }
    return 0;
}
