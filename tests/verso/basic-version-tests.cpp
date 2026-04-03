// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

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
        {
            const version_test_type version{test_should_not_throw<version_test_type>(
                "Construction with parameters should not throw with valid parameters",
                []() {
                    return version_test_type{1, 2, 3};
                },
                version_test_type{})};
            test_equal("Construction with parameters - major number", version.major(),
                       std::uint8_t{1});
            test_equal("Construction with parameters - minor number", version.minor(),
                       std::uint16_t{2});
            test_equal("Construction with parameters - patch number", version.patch(), 3u);
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
        }
    }

private:
    void test_construction_not_throwing(
        const version_test_traits::major_t major, const version_test_traits::minor_t minor,
        const version_test_traits::patch_t patch,
        const std::optional<std::string> prerelease_data = std::nullopt,
        const std::optional<std::string> build_metadata = std::nullopt) {
        test_should_not_throw<version_test_type>(
            "Construction with parameters should not throw with valid parameters",
            [&]() {
                return version_test_type{major, minor, patch, prerelease_data, build_metadata};
            },
            {});
    }
};

VERSO_REGISTER_TEST(BasicVersionTests);

} // namespace verso::tests
