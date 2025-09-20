// Copyright (c) 2025 Andrea Ballestrazzi
#include "test.hpp"

// C++ STL
#include <cassert>
#include <iostream>
#include <span>

constexpr std::string_view ENDL{"\n"};

namespace verso::tests {
void print_help() {
    std::cout << "verso library tests runner" << ENDL;
    std::cout << ENDL;
    std::cout << "-h, --help\tPrints this help page" << ENDL;
    std::cout << "--verbose\tPrints verbose logging" << ENDL;
    std::cout << "-S\t\tPrint successful tests (off by default)" << ENDL;
}
} // namespace verso::tests

int main(int argc, char* argv[]) {
    bool print_successful_tests{};
    bool verbose_output{};
    bool print_help{};

    // Iterate over arguments to see if we need to print also successful tests.
    for (const auto* const arg : std::span(argv, static_cast<std::size_t>(argc))) {
        const std::string_view arg_sv{arg};
        if (arg_sv == "--help" || arg_sv == "-h") {
            print_help = true;
            // Help has always the priority.
            break;
        }

        if (arg_sv == "--verbose") {
            verbose_output = true;
        }

        if (arg_sv == "-S") {
            print_successful_tests = true;
        }
    }

    if (print_help) {
        verso::tests::print_help();
        return 0;
    }

    bool execution_succeeded{true};
    const auto& all_tests{verso::tests::Test::get_all_tests()};
    if (all_tests.empty()) {
        std::cout << "[ WARNING ] No test registered." << ENDL;
        return 0;
    }

    // Simply retrieve all registered tests and run them.
    for (const auto& test : all_tests) {
        assert(test);

        if (verbose_output) {
            std::cout << "[ INFO ] Running test \"" << test->name() << "\"..." << ENDL;
        }
        test->run();

        if (!test->succeeded()) {
            std::cout << "[ ERROR ] Test \"" << test->name() << "\"" << ENDL;
            std::cout << "Reasons:" << ENDL;

            const auto& failures{test->get_failure_reasons()};
            for (const auto& failure : failures) {
                std::cout << "\t- " << failure << ENDL;
            }

            execution_succeeded = false;
        } else if (print_successful_tests || verbose_output) {
            std::cout << "[ OK ] Test \"" << test->name() << "\"" << ENDL;
        }
    }

    if (execution_succeeded) {
        return 0;
    }

    return 1;
}
