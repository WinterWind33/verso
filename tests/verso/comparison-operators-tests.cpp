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
} // namespace verso::tests
