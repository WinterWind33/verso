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

    # Register the test executable with CTest.
    add_test(NAME verso.basic_version_tests COMMAND $<TARGET_FILE:verso-tests> --filter "basic_version class tests" -S)
    set_tests_properties(verso.basic_version_tests PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY $<TARGET_FILE_DIR:verso-tests>
        PASS_REGULAR_EXPRESSION "\\[ OK \\]"
        FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
    )
    add_test(NAME verso.from_string_tests COMMAND $<TARGET_FILE:verso-tests> --filter "from_string() function tests" -S)
    set_tests_properties(verso.from_string_tests PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY $<TARGET_FILE_DIR:verso-tests>
        PASS_REGULAR_EXPRESSION "\\[ OK \\]"
        FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
    )
    add_test(NAME verso.to_string_tests COMMAND $<TARGET_FILE:verso-tests> --filter "to_string() function tests" -S)
    set_tests_properties(verso.to_string_tests PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY $<TARGET_FILE_DIR:verso-tests>
        PASS_REGULAR_EXPRESSION "\\[ OK \\]"
        FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
    )
    add_test(NAME verso.comparison_operators_tests COMMAND $<TARGET_FILE:verso-tests> --filter "comparison operators tests" -S)
    set_tests_properties(verso.comparison_operators_tests PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY $<TARGET_FILE_DIR:verso-tests>
        PASS_REGULAR_EXPRESSION "\\[ OK \\]"
        FAIL_REGULAR_EXPRESSION "\\[ ERROR \\]"
    )

endif() # VERSO_BUILD_TESTS
