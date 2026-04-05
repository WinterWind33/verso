// Copyright (c) 2026 Andrea Ballestrazzi

// C++ STL
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>

#include <verso/verso.hpp>

void try_create_version(const std::uint32_t major, const std::uint32_t minor,
                        const std::uint32_t patch, const std::optional<std::string> pre_release,
                        const std::optional<std::string> build_metadata) {
    try {
        const auto version =
            verso::version{major, minor, patch, std::move(pre_release), std::move(build_metadata)};
        std::cout << "Version created successfully: " << verso::to_string(version) << "\n";
    } catch (const std::exception& e) {
        std::cerr
            << std::format(
                   "Failed to create version with major={}, minor={}, patch={}, pre_release={}, "
                   "build_metadata={}. Error: {}\n",
                   major, minor, patch, pre_release.value_or("nullopt"),
                   build_metadata.value_or("nullopt"), e.what())
            << std::endl;
    }
}

int main() {
    // Valid versions
    try_create_version(1, 0, 0, std::nullopt, std::nullopt);
    try_create_version(1, 2, 3, "alpha", "sha.1234567");

    // Invalid versions
    try_create_version(
        1, 0, 0, "0008",
        std::nullopt); // INVALID!! Leading zeros are not allowed in numeric identifiers.
    try_create_version(
        1, 0, 0, std::nullopt,
        "amazing-build!"); // INVALID!! Build metadata must consist of dot-separated identifiers
                           // containing only alphanumeric characters and hyphens.
    try_create_version(
        1, 0, 0, "alpha.001",
        std::nullopt); // INVALID!! Leading zeros are not allowed in numeric identifiers.

    try_create_version(
        3, 4, 5, "....",
        std::nullopt); // INVALID!! Empty identifiers are not allowed in pre-release data.

    try_create_version(
        2, 1, 0, std::nullopt,
        "build..metadata"); // INVALID!! Empty identifiers are not allowed in build metadata.
    return 0;
}
