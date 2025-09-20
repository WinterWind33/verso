// Copyright (c) 2025 Andrea Ballestrazzi
#include "verso/verso.hpp"

#include "test.hpp"

namespace verso::tests {

class BasicVersionTests final : public Test {
public:
    BasicVersionTests() noexcept : Test{"basic_version class tests"} {}

    void run() override {
        using version_test_traits = version_traits<std::uint8_t, std::uint16_t, std::uint32_t>;
        using version_test_type = basic_version<version_test_traits>;

        // Default construction
        {
            version_test_type version{};
            test_equal("Default construction - major number", version.major(), std::uint8_t{});
            test_equal("Default construction - minor number", version.minor(), std::uint16_t{});
            test_equal("Default construction - patch number", version.patch(), 0u);
        }
        // Construction with parameters
        {
            version_test_type version{1, 2, 3};
            test_equal("Construction with parameters - major number", version.major(),
                       std::uint8_t{1});
            test_equal("Construction with parameters - minor number", version.minor(),
                       std::uint16_t{2});
            test_equal("Construction with parameters - patch number", version.patch(), 3u);
        }
        // Setters
        {
            version_test_type version{};
            version.major(1);
            version.minor(2);
            version.patch(3);
            test_equal("Setters - major number", version.major(), std::uint8_t{1});
            test_equal("Setters - minor number", version.minor(), std::uint16_t{2});
            test_equal("Setters - patch number", version.patch(), 3u);
        }
        // Equality operator
        {
            version_test_type version1{1, 2, 3};
            version_test_type version2{1, 2, 3};
            version_test_type version3{1, 2, 4};
            test_true("Equality operator - equal versions should be equal", version1 == version2);
            test_true("Equality operator - equal versions - operands order should not matter",
                      version2 == version1);
            test_false("Equality operator - different versions should not be equal",
                       version1 == version3);
            test_false("Equality operator - different versions - operands order should not matter",
                       version3 == version1);
        }
    }
};

VERSO_REGISTER_TEST(BasicVersionTests);

} // namespace verso::tests
