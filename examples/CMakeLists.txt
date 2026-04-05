# Copyright (c) 2025-2026 Andrea Ballestrazzi

add_executable(verso-example-basic "basic/main.cpp")
target_link_libraries(verso-example-basic PRIVATE verso)

add_executable(verso-example-comparisons "comparisons/main.cpp")
target_link_libraries(verso-example-comparisons PRIVATE verso)

add_executable(verso-example-from-string "from-string/main.cpp")
target_link_libraries(verso-example-from-string PRIVATE verso)

# This example shows how to generate a version header file at build time using CMake's configure_file function.

set(VERSO_EXAMPLE_GEN_VERSION_MAJOR 1)
set(VERSO_EXAMPLE_GEN_VERSION_MINOR 2)
set(VERSO_EXAMPLE_GEN_VERSION_PATCH 3)
# Comment the next lines and uncomment the following ones to test the behavior of the library
# with invalid pre-release and build metadata. In that case, a compile-time error should be triggered,
# as the generated version would not conform to the SemVer specification.
set(VERSO_EXAMPLE_GEN_VERSION_PRE_RELEASE "alpha")
set(VERSO_EXAMPLE_GEN_VERSION_BUILD_METADATA "build.456")
#set(VERSO_EXAMPLE_GEN_VERSION_PRE_RELEASE "0008") # INVALID!! Leading zeros are not allowed in numeric identifiers.
#set(VERSO_EXAMPLE_GEN_VERSION_BUILD_METADATA "amazing-build!") # INVALID!!

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake-version-generation/version.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.hpp"
)
add_executable(verso-example-cmake-version-generation "cmake-version-generation/main.cpp" "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")
target_link_libraries(verso-example-cmake-version-generation PRIVATE verso)
target_include_directories(verso-example-cmake-version-generation PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

if(UNIX)
    target_compile_options(verso-example-basic PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-comparisons PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-from-string PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-cmake-version-generation PRIVATE -Wall -Wextra -Werror)
endif()
