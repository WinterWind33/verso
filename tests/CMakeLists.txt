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
endif() # VERSO_BUILD_TESTS
