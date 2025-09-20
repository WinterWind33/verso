// Copyright (c) 2025 Andrea Ballestrazzi
#ifndef VERSO_TESTS_TEST_HPP
#define VERSO_TESTS_TEST_HPP

// C++ STL
#include <concepts>
#include <format>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace verso::tests {

/**
 * @brief Concept for string types.
 *
 * @tparam Type The type to check.
 */
template <typename Type>
concept StringType = std::same_as<Type, std::string> || std::same_as<Type, std::string_view>;

/**
 * @brief Concept for types that can be printed using std::to_string.
 *
 * @tparam Type The type to check.
 */
template <typename Type>
concept PrintableNonStringType = !StringType<Type> && requires {
    { std::to_string(std::declval<Type>()) } -> std::convertible_to<std::string>;
};

/**
 * @brief Base class for all the verso tests.
 */
class Test {
public:
    virtual ~Test() noexcept = default;

    /**
     * @brief Construct a new test class with the given test name.
     *  This constructor will throw an exception if the test name is empty.
     *
     * @param testName The name of the test.
     */
    explicit Test(const std::string_view testName) : m_name{testName} {
        if (m_name.empty()) {
            throw std::invalid_argument("The test name cannot be empty.");
        }
    }

    /**
     * @brief Get the all tests object
     *
     * @return A constant reference to the vector containing all the registered tests.
     */
    [[nodiscard]] static const std::vector<std::unique_ptr<Test>>& get_all_tests() noexcept {
        return s_all_tests;
    }

    /**
     * @brief Registers a new test in the test runner.
     *
     * @param test The test to register.
     */
    static void register_test(std::unique_ptr<Test> test);

    /**
     * @brief Locks the registration of new tests. After calling this method,
     *  any attempt to register a new test will throw an exception.
     *  This is useful to prevent tests from being registered after
     *  the test runner has started running the tests.
     */
    static void lock_registration() noexcept;

    /**
     * @brief Retrieves the name of the test.
     *
     * @return The name of the test.
     */
    [[nodiscard]] std::string_view name() const noexcept {
        return m_name;
    }

    /**
     * @brief To be implemented by the derived class to run the test. This method
     *  defines the test logic.
     */
    virtual void run() = 0;

    /**
     * @brief Checks if the test succeeded.
     *
     * @return true if the test succeeded, false otherwise.
     */
    [[nodiscard]] bool succeeded() const noexcept {
        return m_result;
    }

    /**
     * @brief Retrieves the failure reasons of the test.
     *
     * @return A constant reference to the vector containing the failure reasons.
     */
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
     * @param expected The expected value
     * @param actual The actual value
     */
    template <typename Type>
        requires StringType<Type>
    void test_equal(const std::string_view what, const Type& expected, const Type& actual) {
        if (expected != actual) {
            m_failure_reasons.push_back(
                std::format("{}. Expected: '{}', Actual: '{}'", what, expected, actual));
            m_result = false;
        }
    }

    /**
     * @brief Asserts that the two given values are equal.
     *
     * @tparam Type The type of the values to compare.
     * @param what The description of the test.
     * @param expected The expected value
     * @param actual The actual value
     */
    template <typename Type>
        requires std::equality_comparable<Type> && PrintableNonStringType<Type>
    void test_equal(const std::string_view what, const Type& expected, const Type& actual) {
        if (expected != actual) {
            m_failure_reasons.push_back(std::format("{}. Expected: {}, Actual: {}", what,
                                                    std::to_string(expected),
                                                    std::to_string(actual)));
            m_result = false;
        }
    }

    /**
     * @brief Asserts that the two given values are not equal.
     *
     * @tparam Type The type of the values to compare.
     * @param what The description of the test.
     * @param expected The expected value
     * @param actual The actual value
     */
    template <typename Type>
        requires StringType<Type>
    void test_not_equal(const std::string_view what, const Type& expected, const Type& actual) {
        if (expected == actual) {
            m_failure_reasons.push_back(
                std::format("{}. Expected: '{}', Actual: '{}'", what, expected, actual));
            m_result = false;
        }
    }

    /**
     * @brief Asserts that the two given values are not equal.
     *
     * @tparam Type The type of the values to compare.
     * @param what The description of the test.
     * @param expected The expected value
     * @param actual The actual value
     */
    template <typename Type>
        requires std::equality_comparable<Type> && PrintableNonStringType<Type>
    void test_not_equal(const std::string_view what, const Type& expected, const Type& actual) {
        if (expected == actual) {
            m_failure_reasons.push_back(std::format("{}. Expected: {}, Actual: {}", what,
                                                    std::to_string(expected),
                                                    std::to_string(actual)));
            m_result = false;
        }
    }

protected:
    std::string m_name{};

private:
    static std::vector<std::unique_ptr<Test>> s_all_tests;
    static bool s_registration_locked;

    // By default we suppose the test to be successful.
    bool m_result{true};
    std::vector<std::string> m_failure_reasons{};
};

/**
 * @brief Concept for types derived from Test.
 *
 * @tparam TestType The type to test.
 */
template <typename TestType>
concept TestClass = std::derived_from<TestType, Test>;

/**
 * @brief Helper class to register a test at static initialization time.
 *
 * @tparam TestType The type of the test to register.
 */
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
/**
 * @brief Macro to automatically register a test class.
 */
#define VERSO_REGISTER_TEST(TestType) \
    VERSO_REGISTER_TEST_DETAIL(TestType, CONCATENATE(g_testRegistrarInstance_, TestType))

} // namespace verso::tests

#endif // VERSO_TESTS_TEST_HPP
