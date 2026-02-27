// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "test.hpp"

// C++ STL
#include <cassert>
#include <iostream>
#include <span>
#include <string_view>

constexpr char ENDL{'\n'};

namespace verso::tests {
/**
 * @brief Prints the help page for the tests runner.
 */
void print_help() {
    std::cout << "verso library tests runner" << ENDL;
    std::cout << ENDL;
    std::cout << "-h, --help\tPrints this help page" << ENDL;
    std::cout << "--verbose\tPrints verbose logging" << ENDL;
    std::cout << "-S\t\tPrint successful tests (off by default)" << ENDL;
    std::cout << "--list\t\tPrints the list of registered tests and exits" << ENDL;
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

    // Iterate over arguments to see if we need to print also successful tests.
    for (const auto* const arg : std::span(argv, static_cast<std::size_t>(argc))) {
        const std::string_view arg_sv{arg};
        if (arg_sv == "--help" || arg_sv == "-h") {
            print_help = true;
            // Help has always the priority.
            break;
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

    bool execution_succeeded{true};
    const auto& all_tests{verso::tests::Test::get_all_tests()};
    if (all_tests.empty()) {
        verso::tests::print_no_tests_warning();
        return 0;
    }

    // Since we are going to run tests using a for loop, we need to lock the registration
    // of new tests to prevent tests from being registered while we are running them.
    verso::tests::Test::lock_registration();

    // Simply retrieve all registered tests and run them.
    for (const auto& test : all_tests) {
        assert(test);

        if (verbose_output) {
            std::cout << "[ INFO ] Running test \"" << test->name() << "\"..." << ENDL;
        }
        test->run();

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

    if (execution_succeeded) {
        return 0;
    }

    return 1;
}
