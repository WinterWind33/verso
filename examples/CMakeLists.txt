# Copyright (c) 2025-2026 Andrea Ballestrazzi

set(VERSO_EXAMPLES_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/examples)

add_executable(verso-example-basic "basic/main.cpp")
set_target_properties(verso-example-basic
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${VERSO_EXAMPLES_RUNTIME_OUTPUT_DIRECTORY})
target_link_libraries(verso-example-basic PRIVATE verso)
