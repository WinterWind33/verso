// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "test.hpp"

// C++ STL
#include <algorithm>
#include <cassert>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>

constexpr char ENDL{'\n'};

namespace verso::tests {
/**
 * @brief Prints the help page for the tests runner.
 */
void print_help() {
    std::cout << "verso library tests runner" << ENDL;
    std::cout << ENDL;
    std::cout << "-h, --help\t  Prints this help page" << ENDL;
    std::cout << "--verbose\t  Prints verbose logging" << ENDL;
    std::cout << "-S\t\t  Print successful tests (off by default)" << ENDL;
    std::cout << "--list\t\t  Prints the list of registered tests and exits" << ENDL;
    std::cout
        << "--filter <filter> Runs only the tests that exactly match the given filter (can be "
           "specified multiple times)"
        << ENDL;
}

void print_no_tests_warning() {
    std::cout << "[ WARNING ] No tests registered." << ENDL;
}

} // namespace verso::tests

int main(int argc, char* argv[]) {
    bool print_successful_tests{};
    bool verbose_output{};
    bool print_help{};
    bool list_tests_only{};
    bool nextArgFilter{};
    std::vector<std::string_view> filters{};

    // Iterate over arguments to see if we need to print also successful tests.
    for (const auto* const arg : std::span(argv, static_cast<std::size_t>(argc))) {
        const std::string_view arg_sv{arg};
        if (arg_sv == "--help" || arg_sv == "-h") {
            print_help = true;
            nextArgFilter = false;
            // Help has always the priority.
            break;
        }

        if (nextArgFilter) {
            // This means that the previous argument was "--filter", so we are expecting this
            // argument to be the filter for the tests to run.
            if (arg_sv.empty()) {
                std::cerr << "[ ERROR ] The filter argument cannot be empty." << ENDL;
                return 1;
            }

            // Check that the filter doesn't start with a dash, otherwise it would be confused with
            // another argument.
            if (arg_sv.starts_with('-')) {
                std::cerr << "[ ERROR ] The filter argument cannot start with a dash." << ENDL;
                return 1;
            }
            filters.push_back(arg_sv);
            nextArgFilter = false;
        }

        if (arg_sv == "--list") {
            list_tests_only = true;
        }

        if (arg_sv == "--verbose") {
            verbose_output = true;
        }

        if (arg_sv == "-S") {
            print_successful_tests = true;
        }

        if (arg_sv == "--filter") {
            // The next argument should be the filtered test name, so we need to check if it exists
            // and if it is not empty.
            nextArgFilter = true;
            continue;
        }
    }

    if (nextArgFilter) {
        // This means that the last argument was "--filter", but we didn't receive the filter value.
        std::cerr << "[ ERROR ] The --filter argument requires a value." << ENDL;
        return 1;
    }

    if (print_help) {
        // Help has the priority over everything else, so we print it and exit immediately.
        verso::tests::print_help();
        return 0;
    }

    if (list_tests_only) {
        const auto& all_tests{verso::tests::Test::get_all_tests()};
        if (all_tests.empty()) {
            verso::tests::print_no_tests_warning();
            return 0;
        }

        std::cout << "Registered tests:" << ENDL;
        for (const auto& test : all_tests) {
            assert(test);
            std::cout << " - " << test->name() << ENDL;
        }

        // We should not run any test, so we can exit immediately after printing the list of tests.
        return 0;
    }

    const auto& all_tests{verso::tests::Test::get_all_tests()};
    if (all_tests.empty()) {
        verso::tests::print_no_tests_warning();
        return 0;
    }

    // Since we are going to run tests using a for loop, we need to lock the registration
    // of new tests to prevent tests from being registered while we are running them.
    verso::tests::Test::lock_registration();

    if (verbose_output && !filters.empty()) {
        std::cout << "[ INFO ] Running tests with the following filters: " << ENDL;
        for (const auto filter : filters) {
            std::cout << " - \"" << filter << '\"' << ENDL;
        }
    }

    bool execution_succeeded{true}, any_test_run{};
    // Simply retrieve all registered tests and run them.
    for (const auto& test : all_tests) {
        assert(test);

        // If filters are specified, we only run the tests that match at least one filter.
        if (!filters.empty() &&
            std::none_of(std::cbegin(filters), std::cend(filters), [&test](const auto& f) {
                // Filter should match the test name exactly, otherwise it would be
                // confusing.
                return test->name() == f;
            })) {
            if (verbose_output) {
                std::cout << "[ INFO ] Skipping test \"" << test->name()
                          << "\" since it doesn't match any filter." << ENDL;
            }

            continue;
        }

        if (verbose_output) {
            std::cout << "[ INFO ] Running test \"" << test->name() << "\"..." << ENDL;
        }
        test->run();
        any_test_run = true;

        // If the test has not succeeded, print the failure reasons.
        if (!test->succeeded()) {
            std::cout << "[ ERROR ] Test \"" << test->name() << "\"" << ENDL;
            std::cout << "Reasons:" << ENDL;

            const auto& failures{test->get_failure_reasons()};
            for (const auto& failure : failures) {
                std::cout << "\t- " << failure << ENDL;
            }

            // Print a new line to separate tests.
            std::cout << ENDL;

            execution_succeeded = false;
        } else if (print_successful_tests || verbose_output) {
            // Print the successful test only if requested or if we are in verbose mode.
            std::cout << "[ OK ] Test \"" << test->name() << "\"" << ENDL;
        }
    }

    if (!any_test_run) {
        std::cout << "[ ERROR ] No tests were run." << ENDL;
        return 1;
    }

    if (execution_succeeded) {
        return 0;
    }

    return 1;
}
