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

        version_test_type version1{1, 0, 0};
        version_test_type version2{1, 1, 0};
        version_test_type version3{1, 1, 1};
        version_test_type version4{2, 0, 0};

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
    }
};

VERSO_REGISTER_TEST(ComparisonOperatorsTests);
} // namespace verso::tests
