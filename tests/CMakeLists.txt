# Copyright (c) 2025-2026 Andrea Ballestrazzi

if(VERSO_BUILD_TESTS)

    set(VERSO_TESTS_SOURCE_FILES
        test.hpp
        test.cpp
        runner-main.cpp

        # Verso test files
        verso/static-tests.cpp
        verso/basic-version-tests.cpp
        verso/from-string-tests.cpp
        verso/to-string-tests.cpp
        verso/comparison-operators-tests.cpp
    )

    add_executable(verso-tests ${VERSO_TESTS_SOURCE_FILES})
    target_link_libraries(verso-tests PRIVATE verso)
    target_include_directories(verso-tests PRIVATE .)

    # If on Linux or macOS, compile with warnings as errors
    if (UNIX)
        target_compile_options(verso-tests PRIVATE -Wall -Wextra -Werror)
    endif()

    function(verso_add_runner_test name filter)
        add_test(NAME ${name} COMMAND $<TARGET_FILE:verso-tests> --filter "${filter}" -S)
        set_tests_properties(${name} PROPERTIES
            TIMEOUT 10
            WORKING_DIRECTORY $<TARGET_FILE_DIR:verso-tests>
            PASS_REGULAR_EXPRESSION "\\[ OK \\]"
            FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
        )
    endfunction()

    # Register the test executable with CTest.
    verso_add_runner_test(verso.basic_version_tests "basic_version class tests")
    verso_add_runner_test(verso.from_string_tests "from_string() function tests")
    verso_add_runner_test(verso.to_string_tests "to_string() function tests")
    verso_add_runner_test(verso.comparison_operators_tests "comparison operators tests")
    verso_add_runner_test(verso.prerelease_comparison_tests "prerelease comparison tests")
    verso_add_runner_test(verso.buildmetadata_comparison_tests "build metadata comparison tests")

endif() # VERSO_BUILD_TESTS
