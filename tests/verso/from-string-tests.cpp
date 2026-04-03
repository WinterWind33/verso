// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

// C++ STL
#include <format>
#include <optional>
#include <string>
#include <string_view>

#include "test.hpp"

namespace verso::tests {

/**
 * @brief Tests for the from_string function.
 *
 */
class FromStringTests final : public Test {
public:
    FromStringTests() : Test{"from_string() function tests"} {}

    void run() override {
        test_valid_scenario("0.0.0", version{0, 0, 0});
        test_valid_scenario("1.2.3", version{1, 2, 3});
        test_valid_scenario("10.20.30", version{10, 20, 30});

        // Prerelease. Examples from specification
        test_valid_scenario("1.0.0-alpha", version{1, 0, 0, "alpha"});
        test_valid_scenario("1.0.0-alpha.1", version{1, 0, 0, "alpha.1"});
        test_valid_scenario("1.0.0-0.3.7", version{1, 0, 0, "0.3.7"});
        test_valid_scenario("1.0.0-x.7.z.92", version{1, 0, 0, "x.7.z.92"});
        test_valid_scenario("1.0.0-x.y.z.--", version{1, 0, 0, "x.y.z.--"});

        // Build metadata. Examples from specification
        test_valid_scenario("1.0.0-alpha+001", version{1, 0, 0, "alpha", "001"});
        test_valid_scenario("1.0.0+20130313144700",
                            version{1, 0, 0, std::nullopt, "20130313144700"});
        test_valid_scenario("1.0.0-beta+exp.sha.5114f85",
                            version{1, 0, 0, "beta", "exp.sha.5114f85"});
        test_valid_scenario("1.0.0+21AF26D3----117B344092BD",
                            version{1, 0, 0, std::nullopt, "21AF26D3----117B344092BD"});
        // Unfortunately, there is no way to distinguish between pre-release and build metadata if
        // the build metadata comes before the pre-release data, which is not compliant with the
        // specification, but we can still parse it as a valid version with build metadata and no
        // pre-release data.
        test_valid_scenario("1.0.0+metadatabefore-prerelease",
                            version{1, 0, 0, std::nullopt, "metadatabefore-prerelease"});

        // Invalid scenarios
        test_invalid_scenario("");
        test_invalid_scenario("1");
        test_invalid_scenario("1.-89.0");
        test_invalid_scenario("1.2.-3");
        test_invalid_scenario("1.+2.3");
        test_invalid_scenario("1.2.+3");
        test_invalid_scenario("1.2");
        test_invalid_scenario("1.2.3.4");
        test_invalid_scenario("1.2.3-");
        test_invalid_scenario("1.2.3+");
        test_invalid_scenario("01.2.3");
        test_invalid_scenario("1.02.3");
        test_invalid_scenario("1.2.03");
        test_invalid_scenario("1.2.3-01");
        test_invalid_scenario("1.0.0-alpha..1");
        test_invalid_scenario("1.0.0-alpha..");
        test_invalid_scenario("1.0.0-alpha#");
        test_invalid_scenario("1.0.0-alpha+");
        test_invalid_scenario("1.0.0-alpha+001..");
        test_invalid_scenario("1.0.0-alpha+001#");
        test_invalid_scenario("1.0.0+amazing-version!");
        test_invalid_scenario("1.0.0-+");
    }

private:
    void test_invalid_scenario(const std::string_view versionString) {
        const auto result{from_string<version>(versionString)};
        test_false(std::format("Result from \'{}\' should not have a value", versionString),
                   result.has_value());
    }

    void test_valid_scenario(const std::string_view versionString, const version& expected) {
        const auto result{from_string<version>(versionString)};
        if (test_true(std::format("Result from \'{}\' should have a value", versionString),
                      result.has_value())) {
            test_true(std::format("Major version should be {}", expected.major()),
                      result->major() == expected.major());
            test_true(std::format("Minor version should be {}", expected.minor()),
                      result->minor() == expected.minor());
            test_true(std::format("Patch version should be {}", expected.patch()),
                      result->patch() == expected.patch());

            if (const auto expPrerelease{expected.prerelease_data()}; expPrerelease) {
                test_true(std::format("Pre-release data should be '{}'", *expPrerelease),
                          result->prerelease_data() == expected.prerelease_data());
            } else {
                test_true("Pre-release data should be nullopt",
                          !result->prerelease_data().has_value());
            }

            if (const auto expBuildMetadata{expected.build_metadata()}; expBuildMetadata) {
                test_true(std::format("Build metadata should be '{}'", *expBuildMetadata),
                          result->build_metadata() == expected.build_metadata());
            } else {
                test_true("Build metadata should be nullopt",
                          !result->build_metadata().has_value());
            }
        }
    }
};

VERSO_REGISTER_TEST(FromStringTests);
} // namespace verso::tests
