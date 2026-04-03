// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "test.hpp"

// C++ STL
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

bool Test::test_true(const std::string_view what, const bool condition) {
    if (!condition) {
        // Format the failure reason
        m_failure_reasons.push_back(std::format("[test_true] Assertion \"{}\" failed", what));
        m_result = false;
    }
    return condition;
}

bool Test::test_false(const std::string_view what, const bool condition) {
    if (condition) {
        // Format the failure reason
        m_failure_reasons.push_back(std::format("[test_false] Assertion \"{}\" failed", what));
        m_result = false;
    }
    return !condition;
}

bool Test::test_should_throw(const std::string_view what, const std::function<void()>& func) {
    // Test the function throws an exception.
    try {
        func();
        m_failure_reasons.push_back(
            std::format("[test_should_throw] {}. Expected an exception to be thrown, but no "
                        "exception was thrown.",
                        what));
        m_result = false;
    } catch (...) {
        return true;
    }

    return false;
}

bool Test::test_should_not_throw(const std::string_view what,
                                 const std::function<void()>& func) noexcept {
    try {
        func();
    } catch (const std::exception& e) {
        m_failure_reasons.push_back(std::format(
            "[test_should_not_throw] {}. Expected no exception to be thrown, but an exception "
            "was thrown. Exception message: {}",
            what, e.what()));
        m_result = false;
        return false;
    } catch (...) {
        m_failure_reasons.push_back(std::format(
            "[test_should_not_throw] {}. Expected no exception to be thrown, but an unknown "
            "exception was thrown.",
            what));
        m_result = false;
        return false;
    }

    return true;
}

} // namespace verso::tests
