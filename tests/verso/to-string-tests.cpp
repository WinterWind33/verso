// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

// C++ STL
#include <format>
#include <optional>
#include <string>

#include "test.hpp"

namespace verso::tests {
/**
 * @brief Tests for the to_string function.
 *
 */
class ToStringTests final : public Test {
public:
    ToStringTests() : Test{"to_string() function tests"} {}

    void run() override {
        test_result(version{}, "0.0.0");
        test_result(version{1, 2, 3}, "1.2.3");

        // From specification examples
        test_result(version{1, 0, 0, "alpha"}, "1.0.0-alpha");
        test_result(version{1, 0, 0, "alpha.1"}, "1.0.0-alpha.1");
        test_result(version{1, 0, 0, "0.3.7"}, "1.0.0-0.3.7");
        test_result(version{1, 0, 0, "x.7.z.92"}, "1.0.0-x.7.z.92");
        test_result(version{1, 0, 0, "x-y-z.--"}, "1.0.0-x-y-z.--");
        test_result(version{1, 0, 0, "alpha", "001"}, "1.0.0-alpha+001");
        test_result(version{1, 0, 0, std::nullopt, "20130313144700"}, "1.0.0+20130313144700");
        test_result(version{1, 0, 0, "beta", "exp.sha.5114f85"}, "1.0.0-beta+exp.sha.5114f85");
        test_result(version{1, 0, 0, std::nullopt, "21AF26D3----117B344092"},
                    "1.0.0+21AF26D3----117B344092");
    }

private:
    void test_result(const version& ver, const std::string& expected) {
        const auto verStr{to_string(ver)};
        test_equal(
            std::format(
                "to_string() - Version: major: {}, minor: {}, patch: {}, prerelease: {}, build: {}",
                ver.major(), ver.minor(), ver.patch(), ver.prerelease_data().value_or("null"),
                ver.build_metadata().value_or("null")),
            expected, verStr);
    }
};

VERSO_REGISTER_TEST(ToStringTests);
} // namespace verso::tests
