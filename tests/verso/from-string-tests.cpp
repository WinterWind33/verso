// Copyright (c) 2025 Andrea Ballestrazzi
#include "verso/verso.hpp"

#include "test.hpp"

namespace verso::tests {

/**
 * @brief Tests for the from_string function.
 *
 */
class FromStringTests final : public Test {
public:
    FromStringTests() : Test{"from_string() function tests"} {}

    void run() override { // Empty string
        {
            const auto result{from_string<version>("")};
            test_false("Result from empty string should be nullopt", result.has_value());
        }

        // Valid version strings
        {
            const auto result{from_string<version>("1.0.0")};
            if (test_true("Result from \'1.0.0\' should have a value", result.has_value())) {
                test_true("Major version should be 1", result->major() == 1);
                test_true("Minor version should be 0", result->minor() == 0);
                test_true("Patch version should be 0", result->patch() == 0);
            }
        }
        {
            const auto result{from_string<version>("0.1.2")};
            if (test_true("Result from \'0.1.2\' should have a value", result.has_value())) {
                test_true("Major version should be 0", result->major() == 0);
                test_true("Minor version should be 1", result->minor() == 1);
                test_true("Patch version should be 2", result->patch() == 2);
            }
        }
        {
            const auto result{from_string<version>("10.20.30")};
            if (test_true("Result from \'10.20.30\' should have a value", result.has_value())) {
                test_true("Major version should be 10", result->major() == 10);
                test_true("Minor version should be 20", result->minor() == 20);
                test_true("Patch version should be 30", result->patch() == 30);
            }
        }

        // Invalid version strings
        {
            constexpr std::string_view testStr{"1.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // Valid but unsupported semantic versions
        {
            constexpr std::string_view testStr{"1.0.0-alpha"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0.0-beta"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0.0+build.1"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // Missing components
        {
            constexpr std::string_view testStr{"1..0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0."};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{".1.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // Leading zeros
        {
            constexpr std::string_view testStr{"01.0.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.01.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0.01"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // Non-numeric characters
        {
            constexpr std::string_view testStr{"a.b.c"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.b.3"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.2.c"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // Negative numbers (not possible with unsigned types)
        // as per semantic versioning specification
        {
            constexpr std::string_view testStr{"-1.0.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.-1.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0.-1"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        // too much dots
        {
            constexpr std::string_view testStr{"1.0.0.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"...1.0.0"};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
        {
            constexpr std::string_view testStr{"1.0.0..."};
            const auto result{from_string<version>(testStr)};
            test_is_nullopt(testStr, result);
        }
    }

private:
    void test_is_nullopt(const std::string_view versionString, const std::optional<version>& opt) {
        test_false("Result from \'" + std::string(versionString) + "\' should be nullopt",
                   opt.has_value());
    }
};

VERSO_REGISTER_TEST(FromStringTests);
} // namespace verso::tests
