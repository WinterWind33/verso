# Copyright (c) 2025 Andrea Ballestrazzi

if(VERSO_BUILD_TESTS)

set(VERSO_TESTS_SOURCE_FILES
    test.hpp
    test.cpp
    runner-main.cpp

    # Verso test files
)

add_executable(verso-tests ${VERSO_TESTS_SOURCE_FILES})
target_link_libraries(verso-tests PRIVATE verso)
target_include_directories(verso-tests PRIVATE .)
endif() # VERSO_BUILD_TESTS
