// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

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
        {
            const auto verStr{to_string(version{})};
            test_equal("to_string(version{}) result", std::string{"0.0.0"}, verStr);
        }
        {
            constexpr version ver{1, 0, 0};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 0, 0}) result", std::string{"1.0.0"}, verStr);
        }
        {
            constexpr version ver{0, 1, 2};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{0, 1, 2}) result", std::string{"0.1.2"}, verStr);
        }
        {
            constexpr version ver{10, 20, 30};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{10, 20, 30}) result", std::string{"10.20.30"}, verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, "alpha"};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 2, 3, \"alpha\"}) result", std::string{"1.2.3-alpha"},
                       verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, "alpha.1"};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 2, 3, \"alpha.1\"}) result",
                       std::string{"1.2.3-alpha.1"}, verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, "-x-y-z"};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 2, 3, \"-x-y-z\"}) result",
                       std::string{"1.2.3--x-y-z"}, verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, std::nullopt, "21AF26D3----117B344092BD"};
            const auto verStr{to_string(ver)};
            test_equal(
                "to_string(version{1, 2, 3, std::nullopt, \"21AF26D3----117B344092BD\"}) result",
                std::string{"1.2.3+21AF26D3----117B344092BD"}, verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, "alpha.1", "build.123"};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 2, 3, \"alpha.1\", \"build.123\"}) result",
                       std::string{"1.2.3-alpha.1+build.123"}, verStr);
        }
        {
            constexpr constant_version ver{1, 2, 3, std::nullopt, "build.123"};
            const auto verStr{to_string(ver)};
            test_equal("to_string(version{1, 2, 3, std::nullopt, \"build.123\"}) result",
                       std::string{"1.2.3+build.123"}, verStr);
        }
        {
            const version ver{1, 2, 3, "alpha", "build.000005"};
            test_equal("to_string(version{1, 2, 3, \"alpha\", \"build.000005\"})",
                       std::string{"1.2.3-alpha+build.000005"}, to_string(ver));
        }
    }
};

VERSO_REGISTER_TEST(ToStringTests);
} // namespace verso::tests
