// Copyright (c) 2025 Andrea Ballestrazzi
#include "test.hpp"

// C++ STL
#include <stdexcept>

class TestTest : public verso::tests::Test {
public:
    TestTest() : verso::tests::Test("TestTest") {}

    void run() override {
        // Test that the test_true assertion works correctly.
        test_true("True condition should pass", true);
        test_true("False condition should fail", false);
        test_false("False condition should pass", false);
        test_false("True condition should fail", true);
        test_equal("Equal integers should pass", 42, 42);
        test_equal("Unequal integers should fail", 42, 43);
        test_not_equal("Unequal integers should pass", 42, 43);
        test_not_equal("Equal integers should fail", 42, 42);
        test_equal("Equal strings should pass", std::string("hello"), std::string("hello"));
        test_equal("Unequal strings should fail", std::string_view("hello"),
                   std::string_view("world"));
    }
};

namespace verso::tests {

std::vector<std::unique_ptr<Test>> Test::s_all_tests{};
bool Test::s_registration_locked{false};

void Test::register_test(std::unique_ptr<Test> test) {
    if (test == nullptr) {
        throw std::invalid_argument("The test to register cannot be a nullptr.");
    }
    if (s_registration_locked) {
        throw std::runtime_error(
            "Attempting to register a test during runner execution is currently not allowed.");
    }

    s_all_tests.push_back(std::move(test));
}

void Test::lock_registration() noexcept {
    s_registration_locked = true;
}

void Test::test_true(const std::string_view what, const bool condition) {
    if (!condition) {
        m_failure_reasons.emplace_back(what);
        m_result = false;
    }
}

void Test::test_false(const std::string_view what, const bool condition) {
    if (condition) {
        m_failure_reasons.emplace_back(what);
        m_result = false;
    }
}

} // namespace verso::tests
