// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

// C++ STL
#include <cstdint>
#include <format>
#include <optional>
#include <string>

#include "test.hpp"

namespace verso::tests {

class BasicVersionTests final : public Test {
public:
    BasicVersionTests() noexcept : Test{"basic_version class tests"} {}

    using version_test_traits = version_traits<std::uint8_t, std::uint16_t, std::uint32_t>;
    using version_test_type = basic_version<version_test_traits>;

    void run() override {
        // Default construction
        {
            const version_test_type version{};
            test_equal("Default construction - major number", version.major(), std::uint8_t{});
            test_equal("Default construction - minor number", version.minor(), std::uint16_t{});
            test_equal("Default construction - patch number", version.patch(), 0u);
        }
        // Construction with parameters - Valid cases
        {
            test_construction_not_throwing(1, 2, 3);

            // Scenarios taken from the specification examples.
            // Prerelease only
            test_construction_not_throwing(1, 0, 0, "alpha");
            test_construction_not_throwing(1, 0, 0, "alpha.1");
            test_construction_not_throwing(1, 0, 0, "0.3.7");
            test_construction_not_throwing(1, 0, 0, "x.7.z.92");
            test_construction_not_throwing(1, 0, 0, "x-y-z.--");
            // Build metadata
            test_construction_not_throwing(1, 0, 0, "alpha", "001");
            test_construction_not_throwing(1, 0, 0, std::nullopt, "20130313144700");
            test_construction_not_throwing(1, 0, 0, "beta", "exp.sha.5114f85");
            test_construction_not_throwing(1, 0, 0, std::nullopt, "21AF26D3----117B344092BD");
        }
        // Construction with invalid parameters.
        {
            // Prerelease only
            test_construction_throwing(1, 2, 3, "alpha..1");
            test_construction_throwing(1, 2, 3, "alpha.01");
            test_construction_throwing(1, 2, 3, "00000000001");
            test_construction_throwing(1, 2, 3, "alpha.");
            test_construction_throwing(1, 2, 3, ".alpha");
            test_construction_throwing(1, 2, 3, "");
            test_construction_throwing(1, 2, 3, ".");
            test_construction_throwing(1, 2, 3, "!nv@l!d");
            // Build metadata only
            test_construction_throwing(1, 2, 3, std::nullopt, "build..meta");
            test_construction_throwing(1, 2, 3, std::nullopt, "amazing-build!");
            test_construction_throwing(1, 2, 3, std::nullopt, "");
            test_construction_throwing(1, 2, 3, std::nullopt, ".");
            test_construction_throwing(1, 2, 3, std::nullopt, "build.");
            test_construction_throwing(1, 2, 3, std::nullopt, ".build");
            test_construction_throwing(1, 2, 3, std::nullopt, "..........");
            test_construction_throwing(1, 2, 3, std::nullopt, "%$#");
        }
        // Getters tests
        {
            const version_test_type ver{1, 2, 3, "alpha.1", "build.123"};
            test_equal("Construction with parameters - major number", ver.major(), std::uint8_t{1});
            test_equal("Construction with parameters - minor number", ver.minor(),
                       std::uint16_t{2});
            test_equal("Construction with parameters - patch number", ver.patch(), 3u);

            if (test_true("Construction with parameters - prerelease data should be present",
                          ver.prerelease_data().has_value())) {
                test_equal("Construction with parameters - prerelease data value",
                           ver.prerelease_data().value(), std::string{"alpha.1"});
            }
            if (test_true("Construction with parameters - build metadata should be present",
                          ver.build_metadata().has_value())) {
                test_equal("Construction with parameters - build metadata value",
                           ver.build_metadata().value(), std::string{"build.123"});
            }
        }
        // Setters tests
        {
            version_test_type ver{};
            ver.major(1);
            ver.minor(2);
            ver.patch(3);

            test_true("Setters - major number should be set correctly", ver.major() == 1);
            test_true("Setters - minor number should be set correctly", ver.minor() == 2);
            test_true("Setters - patch number should be set correctly", ver.patch() == 3);

            // Test valid prerelease data and build metadata.
            test_valid_prerelease_set(ver, "alpha");
            test_valid_prerelease_set(ver, "alpha.1");
            test_valid_prerelease_set(ver, "0.3.7");
            test_valid_prerelease_set(ver, "x.7.z.92");
            test_valid_prerelease_set(ver, "x-y-z.--");
            test_valid_prerelease_set(ver, "0alpha");
            test_valid_prerelease_set(ver, "001alpha");
            test_valid_build_metadata_set(ver, "001");
            test_valid_build_metadata_set(ver, "001build");
            test_valid_build_metadata_set(ver, "20130313144700");
            test_valid_build_metadata_set(ver, "exp.sha.5114f85");
            test_valid_build_metadata_set(ver, "21AF26D3----117B344092");

            // Invalid scenarios
            test_invalid_prerelease_set(ver, "alpha..1");
            test_invalid_prerelease_set(ver, "alpha.01");
            test_invalid_prerelease_set(ver, "00000000001");
            test_invalid_prerelease_set(ver, "alpha.");
            test_invalid_prerelease_set(ver, ".alpha");
            test_invalid_prerelease_set(ver, "");
            test_invalid_prerelease_set(ver, ".");
            test_invalid_prerelease_set(ver, "!nv@l!d");
            test_invalid_build_metadata_set(ver, "build..meta");
            test_invalid_build_metadata_set(ver, "amazing-build!");
            test_invalid_build_metadata_set(ver, "");
            test_invalid_build_metadata_set(ver, ".");
            test_invalid_build_metadata_set(ver, "build.");
            test_invalid_build_metadata_set(ver, ".build");
            test_invalid_build_metadata_set(ver, "..........");
            test_invalid_build_metadata_set(ver, "%$#");
        }
        // Equality operator (on core version data)
        {
            const version_test_type version1{1, 2, 3};
            const version_test_type version2{1, 2, 3};
            const version_test_type version3{1, 2, 4};
            test_true("Equality operator - equal versions should be equal", version1 == version2);
            test_true("Equality operator - equal versions - operands order should not matter",
                      version2 == version1);
            test_false("Equality operator - different versions should not be equal",
                       version1 == version3);
            test_false("Equality operator - different versions - operands order should not matter",
                       version3 == version1);

            // Prerelease data
            const version_test_type version4{1, 2, 3, "alpha"};
            const version_test_type version5{1, 2, 3, "alpha"};
            const version_test_type version6{1, 2, 3, "beta"};
            test_true("Equality operator - equal versions with prerelease data should be equal",
                      version4 == version5);
            test_true(
                "Equality operator - equal versions with prerelease data - operands order should "
                "not matter",
                version5 == version4);
            test_false(
                "Equality operator - different versions with prerelease data should not be equal",
                version4 == version6);
            test_false(
                "Equality operator - different versions with prerelease data - operands order "
                "should not matter",
                version6 == version4);

            // Build metadata should be ignored for equality purposes.
            const version_test_type version7{1, 2, 3, "alpha", "build.123"};
            const version_test_type version8{1, 2, 3, "alpha", "exp.sha.5114f85"};
            test_true("Equality operator - equal versions with build metadata should be equal",
                      version7 == version8);
            test_true(
                "Equality operator - equal versions with build metadata - operands order should "
                "not matter",
                version8 == version7);
        }
    }

private:
    void test_construction_not_throwing(
        const version_test_traits::major_t major, const version_test_traits::minor_t minor,
        const version_test_traits::patch_t patch,
        const std::optional<std::string> prerelease_data = std::nullopt,
        const std::optional<std::string> build_metadata = std::nullopt) {
        test_should_not_throw<version_test_type>(
            std::format(
                "Construction with parameters should not throw with valid parameters - major: {}, "
                "minor: {}, patch: {}, prerelease_data: {}, build_metadata: {}",
                major, minor, patch, prerelease_data.value_or("null"),
                build_metadata.value_or("null")),
            [&]() {
                return version_test_type{major, minor, patch, prerelease_data, build_metadata};
            },
            {});
    }

    void test_construction_throwing(
        const version_test_traits::major_t major, const version_test_traits::minor_t minor,
        const version_test_traits::patch_t patch,
        const std::optional<std::string> prerelease_data = std::nullopt,
        const std::optional<std::string> build_metadata = std::nullopt) {
        test_should_throw(
            std::format(
                "Construction with parameters should throw with invalid parameters - major: {}, "
                "minor: {}, patch: {}, prerelease_data: {}, build_metadata: {}",
                major, minor, patch, prerelease_data.value_or("null"),
                build_metadata.value_or("null")),
            [&]() {
                return version_test_type{major, minor, patch, prerelease_data, build_metadata};
            });
    }

    void test_valid_prerelease_set(version_test_type& ver, const std::string& prerelease_data) {
        test_should_not_throw(
            std::format(
                "Setters - prerelease data should be set without throwing with valid value '{}'",
                prerelease_data),
            [&]() {
                ver.prerelease_data(prerelease_data);
            });
        if (test_true(std::format(
                          "Setters - prerelease data should be set correctly with valid value '{}'",
                          prerelease_data),
                      ver.prerelease_data().has_value())) {
            test_equal(
                std::format("Setters - prerelease data value should be correct when setting '{}'",
                            prerelease_data),
                ver.prerelease_data().value(), prerelease_data);
        }
    }

    void test_invalid_prerelease_set(version_test_type& ver, const std::string& prerelease_data) {
        test_should_throw(
            std::format("Setters - prerelease data should throw when setting invalid value '{}'",
                        prerelease_data),
            [&]() {
                ver.prerelease_data(prerelease_data);
            });
    }

    void test_invalid_build_metadata_set(version_test_type& ver,
                                         const std::string& build_metadata) {
        test_should_throw(
            std::format("Setters - build metadata should throw when setting invalid value '{}'",
                        build_metadata),
            [&]() {
                ver.build_metadata(build_metadata);
            });
    }

    void test_valid_build_metadata_set(version_test_type& ver, const std::string& build_metadata) {
        test_should_not_throw(
            std::format(
                "Setters - build metadata should be set without throwing with valid value '{}'",
                build_metadata),
            [&]() {
                ver.build_metadata(build_metadata);
            });
        if (test_true(std::format(
                          "Setters - build metadata should be set correctly with valid value '{}'",
                          build_metadata),
                      ver.build_metadata().has_value())) {
            test_equal(
                std::format("Setters - build metadata value should be correct when setting '{}'",
                            build_metadata),
                ver.build_metadata().value(), build_metadata);
        }
    }
};

VERSO_REGISTER_TEST(BasicVersionTests);

} // namespace verso::tests
