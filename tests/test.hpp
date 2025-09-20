// Copyright (c) 2025 Andrea Ballestrazzi
#ifndef VERSO_TESTS_TEST_HPP
#define VERSO_TESTS_TEST_HPP

// C++ STL
#include <concepts>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace verso::tests {

/**
 * @brief Default name for unnamed tests.
 */
constexpr std::string_view TESTS_DEFAULT_NAME{"Unnamed Test"};

/**
 * @brief Concept for types that can be printed using std::to_string.
 *
 * @tparam Type The type to test.
 */
template <typename Type>
concept PrintableType = requires {
    { std::to_string(std::declval<Type>()) } -> std::convertible_to<std::string>;
};

class Test {
public:
    [[nodiscard]] static const std::vector<std::unique_ptr<Test>>& get_all_tests() noexcept {
        return s_all_tests;
    }

    static void register_test(std::unique_ptr<Test> test);

    [[nodiscard]] std::string_view name() const noexcept {
        return m_name;
    }

    /**
     * @brief To be implemented by the derived class to run the test. This method
     *  defines the test logic.
     */
    virtual void run() = 0;

    [[nodiscard]] bool succeeded() const noexcept {
        return m_result;
    }

    [[nodiscard]] const auto& get_failure_reasons() const noexcept {
        return m_failure_reasons;
    }

    // ### Assertions ###

    /**
     * @brief Asserts that the given condition is true.
     *
     * @param what The description of the test.
     * @param condition The condition to test.
     */
    void test_true(std::string_view what, bool condition);

    /**
     * @brief Asserts that the given condition is false.
     *
     * @param what The description of the test.
     * @param condition The condition to test.
     */
    void test_false(std::string_view what, bool condition);

    /**
     * @brief Asserts that the two given values are equal.
     *
     * @tparam Type The type of the values to compare.
     * @param what The description of the test.
     * @param lhs The first value to compare
     * @param rhs The second value to compare
     */
    template <typename Type>
        requires std::equality_comparable<Type> && PrintableType<Type>
    void test_equal(std::string_view what, const Type& lhs, const Type& rhs) {
        if (lhs != rhs) {
            m_failure_reasons.push_back(std::string{what} + ". Expected: " + std::to_string(lhs) +
                                        ", actual: " + std::to_string(rhs));
            m_result = false;
        }
    }

    /**
     * @brief Asserts that the two given values are not equal.
     *
     * @tparam Type The type of the values to compare.
     * @param what The description of the test.
     * @param lhs The first value to compare
     * @param rhs The second value to compare
     */
    template <typename Type>
        requires std::equality_comparable<Type> && PrintableType<Type>
    void test_not_equal(std::string_view what, const Type& lhs, const Type& rhs) {
        if (lhs == rhs) {
            m_failure_reasons.push_back(std::string{what} + ". Expected: " + std::to_string(lhs) +
                                        ", actual: " + std::to_string(rhs));
            m_result = false;
        }
    }

protected:
    std::string m_name{TESTS_DEFAULT_NAME};

private:
    static std::vector<std::unique_ptr<Test>> s_all_tests;

    // By default we suppose the test to be successful.
    bool m_result{true};
    std::vector<std::string> m_failure_reasons{};
};

template <typename TestType>
concept TestClass = std::derived_from<TestType, Test>;

template <TestClass TestType>
class TestRegistrar {
public:
    explicit TestRegistrar() {
        Test::register_test(std::make_unique<TestType>());
    }
};

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define VERSO_REGISTER_TEST_DETAIL(TestType, InstanceName)         \
    namespace {                                                    \
    static ::verso::tests::TestRegistrar<TestType> InstanceName{}; \
    }
#define VERSO_REGISTER_TEST(TestType) \
    VERSO_REGISTER_TEST_DETAIL(TestType, CONCATENATE(g_testRegistrarInstance_, TestType))

} // namespace verso::tests

#endif // VERSO_TESTS_TEST_HPP
