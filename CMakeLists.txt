# Copyright (c) 2025-2026 Andrea Ballestrazzi
cmake_minimum_required(VERSION 3.19)

project(verso
    VERSION 0.1.0
    DESCRIPTION "A C++ library for semantic versioning"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(VERSO_BUILD_TESTS "Build the tests." OFF)

set(VERSO_INCLUDE_FILES
    include/verso/verso.hpp
)

add_library(verso INTERFACE ${VERSO_INCLUDE_FILES})
target_include_directories(verso INTERFACE include)

if (VERSO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
