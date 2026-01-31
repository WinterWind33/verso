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
    }
};

VERSO_REGISTER_TEST(ToStringTests);
} // namespace verso::tests
