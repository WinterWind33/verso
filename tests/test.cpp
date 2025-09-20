// Copyright (c) 2025 Andrea Ballestrazzi
#include "test.hpp"

#include <stdexcept>

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

void Test::test_true(std::string_view what, bool condition) {
    if (!condition) {
        m_failure_reasons.emplace_back(what);
        m_result = false;
    }
}

void Test::test_false(std::string_view what, bool condition) {
    if (condition) {
        m_failure_reasons.emplace_back(what);
        m_result = false;
    }
}

} // namespace verso::tests
