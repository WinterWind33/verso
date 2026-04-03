// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

#include "test.hpp"

namespace verso::tests {

class ComparisonOperatorsTests final : public Test {
public:
    ComparisonOperatorsTests() noexcept : Test{"comparison operators tests"} {}

    void run() override {
        using version_test_traits = version_traits<std::uint8_t, std::uint16_t, std::uint32_t>;
        using version_test_type = basic_version<version_test_traits>;

        const version_test_type version1{1, 0, 0};
        const version_test_type version2{1, 1, 0};
        const version_test_type version3{1, 1, 1};
        const version_test_type version4{2, 0, 0};

        // Strictly greater than operator
        {
            test_false("version1 > version1", version1 > version1);
            test_false("version1 > version2", version1 > version2);
            test_false("version1 > version3", version1 > version3);
            test_false("version1 > version4", version1 > version4);

            test_true("version2 > version1", version2 > version1);
            test_false("version2 > version2", version2 > version2);
            test_false("version2 > version3", version2 > version3);
            test_false("version2 > version4", version2 > version4);

            test_true("version3 > version1", version3 > version1);
            test_true("version3 > version2", version3 > version2);
            test_false("version3 > version3", version3 > version3);
            test_false("version3 > version4", version3 > version4);

            test_true("version4 > version3", version4 > version3);
            test_true("version4 > version2", version4 > version2);
            test_true("version4 > version1", version4 > version1);
            test_false("version4 > version4", version4 > version4);
        }

        // Strictly less than operator
        {
            test_false("version1 < version1", version1 < version1);
            test_true("version1 < version2", version1 < version2);
            test_true("version1 < version3", version1 < version3);
            test_true("version1 < version4", version1 < version4);

            test_false("version2 < version1", version2 < version1);
            test_false("version2 < version2", version2 < version2);
            test_true("version2 < version3", version2 < version3);
            test_true("version2 < version4", version2 < version4);

            test_false("version3 < version1", version3 < version1);
            test_false("version3 < version2", version3 < version2);
            test_false("version3 < version3", version3 < version3);
            test_true("version3 < version4", version3 < version4);

            test_false("version4 < version3", version4 < version3);
            test_false("version4 < version2", version4 < version2);
            test_false("version4 < version1", version4 < version1);
            test_false("version4 < version4", version4 < version4);
        }

        // Greater-than-or-equal operator
        {
            test_true("version1 >= version1", version1 >= version1);
            test_false("version1 >= version2", version1 >= version2);
            test_false("version1 >= version3", version1 >= version3);
            test_false("version1 >= version4", version1 >= version4);

            test_true("version2 >= version1", version2 >= version1);
            test_true("version2 >= version2", version2 >= version2);
            test_false("version2 >= version3", version2 >= version3);
            test_false("version2 >= version4", version2 >= version4);

            test_true("version3 >= version1", version3 >= version1);
            test_true("version3 >= version2", version3 >= version2);
            test_true("version3 >= version3", version3 >= version3);
            test_false("version3 >= version4", version3 >= version4);

            test_true("version4 >= version3", version4 >= version3);
            test_true("version4 >= version2", version4 >= version2);
            test_true("version4 >= version1", version4 >= version1);
            test_true("version4 >= version4", version4 >= version4);
        }

        // Less-than-or-equal operator
        {
            test_true("version1 <= version1", version1 <= version1);
            test_true("version1 <= version2", version1 <= version2);
            test_true("version1 <= version3", version1 <= version3);
            test_true("version1 <= version4", version1 <= version4);

            test_false("version2 <= version1", version2 <= version1);
            test_true("version2 <= version2", version2 <= version2);
            test_true("version2 <= version3", version2 <= version3);
            test_true("version2 <= version4", version2 <= version4);

            test_false("version3 <= version1", version3 <= version1);
            test_false("version3 <= version2", version3 <= version2);
            test_true("version3 <= version3", version3 <= version3);
            test_true("version3 <= version4", version3 <= version4);

            test_false("version4 <= version3", version4 <= version3);
            test_false("version4 <= version2", version4 <= version2);
            test_false("version4 <= version1", version4 <= version1);
            test_true("version4 <= version4", version4 <= version4);
        }

        // Three-way comparison operator
        {
            test_true("version1 <=> version1 == std::strong_ordering::equal",
                      (version1 <=> version1) == std::strong_ordering::equal);
            test_true("version1 <=> version2 == std::strong_ordering::less",
                      (version1 <=> version2) == std::strong_ordering::less);
            test_true("version1 <=> version3 == std::strong_ordering::less",
                      (version1 <=> version3) == std::strong_ordering::less);
            test_true("version1 <=> version4 == std::strong_ordering::less",
                      (version1 <=> version4) == std::strong_ordering::less);

            test_true("version2 <=> version1 == std::strong_ordering::greater",
                      (version2 <=> version1) == std::strong_ordering::greater);
            test_true("version2 <=> version2 == std::strong_ordering::equal",
                      (version2 <=> version2) == std::strong_ordering::equal);
            test_true("version2 <=> version3 == std::strong_ordering::less",
                      (version2 <=> version3) == std::strong_ordering::less);
            test_true("version2 <=> version4 == std::strong_ordering::less",
                      (version2 <=> version4) == std::strong_ordering::less);

            test_true("version3 <=> version1 == std::strong_ordering::greater",
                      (version3 <=> version1) == std::strong_ordering::greater);
            test_true("version3 <=> version2 == std::strong_ordering::greater",
                      (version3 <=> version2) == std::strong_ordering::greater);
            test_true("version3 <=> version3 == std::strong_ordering::equal",
                      (version3 <=> version3) == std::strong_ordering::equal);
            test_true("version3 <=> version4 == std::strong_ordering::less",
                      (version3 <=> version4) == std::strong_ordering::less);

            test_true("version4 <=> version1 == std::strong_ordering::greater",
                      (version4 <=> version1) == std::strong_ordering::greater);
            test_true("version4 <=> version2 == std::strong_ordering::greater",
                      (version4 <=> version2) == std::strong_ordering::greater);
            test_true("version4 <=> version3 == std::strong_ordering::greater",
                      (version4 <=> version3) == std::strong_ordering::greater);
            test_true("version4 <=> version4 == std::strong_ordering::equal",
                      (version4 <=> version4) == std::strong_ordering::equal);
        }
    }
};

VERSO_REGISTER_TEST(ComparisonOperatorsTests);

class PrereleaseComparisonTests final : public Test {
public:
    PrereleaseComparisonTests() noexcept : Test{"prerelease comparison tests"} {}

    void run() override {
        using version_test_type = version;

        // All the following tests respect the specification at: https://semver.org/#spec-item-11

        const version_test_type version1{1, 0, 0};
        const version_test_type version2{1, 0, 0, "alpha"};
        const version_test_type version3{1, 0, 0, "beta"};
        const version_test_type version4{1, 0, 0, "45"};
        const version_test_type version5{1, 0, 0, "3"};
        const version_test_type version6{1, 0, 0, "alpha.14.789"};

        // Strictly greater than operator
        {
            test_true("version1 > version2", version1 > version2);
            test_true("version1 > version3", version1 > version3);
            test_true("version1 > version4", version1 > version4);
            test_true("version1 > version5", version1 > version5);
            test_true("version1 > version6", version1 > version6);

            test_false("version2 > version1", version2 > version1);
            test_false("version2 > version3", version2 > version3);
            // Pre-release identifiers consisting of only digits have lower precedence than those
            // with letters, so "alpha" > "45" (Specification 11.4.3)
            test_true("version2 > version4", version2 > version4);
            test_true("version2 > version5", version2 > version5);

            // In this case, rule 11.4.4 applies, so version 6 is greater than version 2, because
            // "alpha.14.789" has more identifiers than "alpha", and the first 1 identifier is
            // equal.
            test_false("version2 > version6", version2 > version6);

            test_false("version3 > version1", version3 > version1);
            // Rule 11.4.2 applies, so "beta" > "alpha"
            test_true("version3 > version2", version3 > version2);
            test_true("version3 > version4",
                      version3 > version4); // Rule 11.4.3 applies, so "beta" > "45"
            test_true("version3 > version5",
                      version3 > version5); // Rule 11.4.3 applies, so "beta" > "3"
            test_true("version3 > version6",
                      version3 > version6); // Rule 11.4.2 applies, so "beta" > "alpha.14.789"

            test_false("version4 > version1", version4 > version1);
            test_false("version4 > version2", version4 > version2);
            test_false("version4 > version3", version4 > version3);
            // Rule 11.4.1 applies, so "45" > "3"
            test_true("version4 > version5", version4 > version5);
            test_false("version4 > version6", version4 > version6);

            test_false("version5 > version1", version5 > version1);
            test_false("version5 > version2", version5 > version2);
            test_false("version5 > version3", version5 > version3);
            test_false("version5 > version4", version5 > version4);
            test_false("version5 > version6", version5 > version6);

            test_false("version6 > version1", version6 > version1);
            // Rule 11.4.4 applies, so "alpha.14.789" > "alpha"
            test_true("version6 > version2", version6 > version2);
            // Rule 11.4.2 applies, so "alpha.14.789" > "beta"
            test_false("version6 > version3", version6 > version3);
            // Rule 11.4.3 applies, so "alpha.14.789" > "45"
            test_true("version6 > version4", version6 > version4);
            // Rule 11.4.3 applies, so "alpha.14.789" > "3"
            test_true("version6 > version5", version6 > version5);
        }

        // Greater or equal operator
        {
            test_true("version1 >= version1", version1 >= version1);
            test_true("version1 >= version2", version1 >= version2);
            test_true("version1 >= version3", version1 >= version3);
            test_true("version1 >= version4", version1 >= version4);
            test_true("version1 >= version5", version1 >= version5);
            test_true("version1 >= version6", version1 >= version6);

            test_false("version2 >= version1", version2 >= version1);
            test_true("version2 >= version2", version2 >= version2);
            test_false("version2 >= version3", version2 >= version3);
            test_true("version2 >= version4", version2 >= version4);
            test_true("version2 >= version5", version2 >= version5);
            test_false("version2 >= version6", version2 >= version6);

            test_false("version3 >= version1", version3 >= version1);
            test_true("version3 >= version2", version3 >= version2);
            test_true("version3 >= version3", version3 >= version3);
            test_true("version3 >= version4", version3 >= version4);
            test_true("version3 >= version5", version3 >= version5);
            test_true("version3 >= version6", version3 >= version6);

            test_false("version4 >= version1", version4 >= version1);
            test_false("version4 >= version2", version4 >= version2);
            test_false("version4 >= version3", version4 >= version3);
            test_true("version4 >= version4", version4 >= version4);
            test_true("version4 >= version5", version4 >= version5);
            test_false("version4 >= version6", version4 >= version6);

            test_false("version5 >= version1", version5 >= version1);
            test_false("version5 >= version2", version5 >= version2);
            test_false("version5 >= version3", version5 >= version3);
            test_false("version5 >= version4", version5 >= version4);
            test_true("version5 >= version5", version5 >= version5);
            test_false("version5 >= version6", version5 >= version6);

            test_false("version6 >= version1", version6 >= version1);
            test_true("version6 >= version2", version6 >= version2);
            test_false("version6 >= version3", version6 >= version3);
            test_true("version6 >= version4", version6 >= version4);
            test_true("version6 >= version5", version6 >= version5);
            test_true("version6 >= version6", version6 >= version6);
        }

        // Three-way comparison operator
        {
            test_true("version1 <=> version1 == std::strong_ordering::equal",
                      (version1 <=> version1) == std::strong_ordering::equal);
            test_true("version1 <=> version2 == std::strong_ordering::greater",
                      (version1 <=> version2) == std::strong_ordering::greater);
            test_true("version1 <=> version3 == std::strong_ordering::greater",
                      (version1 <=> version3) == std::strong_ordering::greater);
            test_true("version1 <=> version4 == std::strong_ordering::greater",
                      (version1 <=> version4) == std::strong_ordering::greater);
            test_true("version1 <=> version5 == std::strong_ordering::greater",
                      (version1 <=> version5) == std::strong_ordering::greater);
            test_true("version1 <=> version6 == std::strong_ordering::greater",
                      (version1 <=> version6) == std::strong_ordering::greater);

            test_true("version2 <=> version1 == std::strong_ordering::less",
                      (version2 <=> version1) == std::strong_ordering::less);
            test_true("version2 <=> version2 == std::strong_ordering::equal",
                      (version2 <=> version2) == std::strong_ordering::equal);
            test_true("version2 <=> version3 == std::strong_ordering::less",
                      (version2 <=> version3) == std::strong_ordering::less);
            test_true("version2 <=> version4 == std::strong_ordering::greater",
                      (version2 <=> version4) == std::strong_ordering::greater);
            test_true("version2 <=> version5 == std::strong_ordering::greater",
                      (version2 <=> version5) == std::strong_ordering::greater);
            test_true("version2 <=> version6 == std::strong_ordering::less",
                      (version2 <=> version6) == std::strong_ordering::less);

            test_true("version3 <=> version1 == std::strong_ordering::less",
                      (version3 <=> version1) == std::strong_ordering::less);
            test_true("version3 <=> version2 == std::strong_ordering::greater",
                      (version3 <=> version2) == std::strong_ordering::greater);
            test_true("version3 <=> version3 == std::strong_ordering::equal",
                      (version3 <=> version3) == std::strong_ordering::equal);
            test_true("version3 <=> version4 == std::strong_ordering::greater",
                      (version3 <=> version4) == std::strong_ordering::greater);
            test_true("version3 <=> version5 == std::strong_ordering::greater",
                      (version3 <=> version5) == std::strong_ordering::greater);
            test_true("version3 <=> version6 == std::strong_ordering::greater",
                      (version3 <=> version6) == std::strong_ordering::greater);

            test_true("version4 <=> version1 == std::strong_ordering::less",
                      (version4 <=> version1) == std::strong_ordering::less);
            test_true("version4 <=> version2 == std::strong_ordering::less",
                      (version4 <=> version2) == std::strong_ordering::less);
            test_true("version4 <=> version3 == std::strong_ordering::less",
                      (version4 <=> version3) == std::strong_ordering::less);
            test_true("version4 <=> version4 == std::strong_ordering::equal",
                      (version4 <=> version4) == std::strong_ordering::equal);
            test_true("version4 <=> version5 == std::strong_ordering::greater",
                      (version4 <=> version5) == std::strong_ordering::greater);
            test_true("version4 <=> version6 == std::strong_ordering::less",
                      (version4 <=> version6) == std::strong_ordering::less);

            test_true("version5 <=> version1 == std::strong_ordering::less",
                      (version5 <=> version1) == std::strong_ordering::less);
            test_true("version5 <=> version2 == std::strong_ordering::less",
                      (version5 <=> version2) == std::strong_ordering::less);
            test_true("version5 <=> version3 == std::strong_ordering::less",
                      (version5 <=> version3) == std::strong_ordering::less);
            test_true("version5 <=> version4 == std::strong_ordering::less",
                      (version5 <=> version4) == std::strong_ordering::less);
            test_true("version5 <=> version5 == std::strong_ordering::equal",
                      (version5 <=> version5) == std::strong_ordering::equal);
            test_true("version5 <=> version6 == std::strong_ordering::less",
                      (version5 <=> version6) == std::strong_ordering::less);

            test_true("version6 <=> version1 == std::strong_ordering::less",
                      (version6 <=> version1) == std::strong_ordering::less);
            test_true("version6 <=> version2 == std::strong_ordering::greater",
                      (version6 <=> version2) == std::strong_ordering::greater);
            test_true("version6 <=> version3 == std::strong_ordering::less",
                      (version6 <=> version3) == std::strong_ordering::less);
            test_true("version6 <=> version4 == std::strong_ordering::greater",
                      (version6 <=> version4) == std::strong_ordering::greater);
            test_true("version6 <=> version5 == std::strong_ordering::greater",
                      (version6 <=> version5) == std::strong_ordering::greater);
            test_true("version6 <=> version6 == std::strong_ordering::equal",
                      (version6 <=> version6) == std::strong_ordering::equal);
        }
    }
};

VERSO_REGISTER_TEST(PrereleaseComparisonTests);

class BuildMetadataComparisonTests final : public Test {
public:
    BuildMetadataComparisonTests() noexcept : Test{"build metadata comparison tests"} {}

    void run() override {
        using version_test_type = version;

        // Build metadata should not affect the precedence of versions, so all the following
        // comparisons should return equal.

        // Core version data
        {
            const version_test_type version1{1, 0, 0};
            const version_test_type version2{1, 0, 0, std::nullopt, "build.123"};
            const version_test_type version3{1, 0, 0, std::nullopt, "build.456.E08F45A"};

            // As per specification https://semver.org/#spec-item-10, build metadata should not
            // affect the precedence of versions, so all the above versions should be considered
            // equal.
            test_true("version1 == version2", version1 == version2);
            test_true("version1 == version3", version1 == version3);
            test_true("version2 == version3", version2 == version3);

            test_true("version1 <=> version2 == std::strong_ordering::equal",
                      (version1 <=> version2) == std::strong_ordering::equal);
            test_true("version1 <=> version3 == std::strong_ordering::equal",
                      (version1 <=> version3) == std::strong_ordering::equal);
            test_true("version2 <=> version3 == std::strong_ordering::equal",
                      (version2 <=> version3) == std::strong_ordering::equal);
        }

        // Versions with prerelease
        {
            const version_test_type version1{1, 0, 0, "alpha"};
            const version_test_type version2{1, 0, 0, "alpha", "build.123"};
            const version_test_type version3{1, 0, 0, "alpha", "build.456.E08F45A"};

            // As per specification https://semver.org/#spec-item-10, build metadata should not
            // affect the precedence of versions, so all the above versions should be considered
            // equal.
            test_true("version1 == version2", version1 == version2);
            test_true("version1 == version3", version1 == version3);
            test_true("version2 == version3", version2 == version3);

            test_true("version1 <=> version2 == std::strong_ordering::equal",
                      (version1 <=> version2) == std::strong_ordering::equal);
            test_true("version1 <=> version3 == std::strong_ordering::equal",
                      (version1 <=> version3) == std::strong_ordering::equal);
            test_true("version2 <=> version3 == std::strong_ordering::equal",
                      (version2 <=> version3) == std::strong_ordering::equal);
        }
    }
};

VERSO_REGISTER_TEST(BuildMetadataComparisonTests);

} // namespace verso::tests
