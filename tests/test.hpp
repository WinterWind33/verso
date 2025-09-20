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

constexpr std::string_view TESTS_DEFAULT_NAME{"Unnamed Test"};

class Test {
public:
    Test() noexcept = default;

    [[nodiscard]] std::string_view name() const noexcept {
        return m_name;
    }

    virtual void run() = 0;

    [[nodiscard]] bool succeeded() const noexcept {
        return m_result;
    }

    const auto& get_failure_reasons() const noexcept {
        return m_failure_reasons;
    }

    static const std::vector<std::unique_ptr<Test>>& get_all_tests() noexcept {
        return s_all_tests;
    }

    static void register_test(std::unique_ptr<Test> test);

protected:
    std::string m_name{TESTS_DEFAULT_NAME};

private:
    static std::vector<std::unique_ptr<Test>> s_all_tests;

    bool m_result{};
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
#define VERSO_REGISTER_TEST(TestType) \
    static ::verso::tests::TestRegistrar<TestType> CONCATENATE(testRegistrar_, #TestType) {}

} // namespace verso::tests

#endif // VERSO_TESTS_TEST_HPP
