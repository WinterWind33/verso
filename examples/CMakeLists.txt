# Copyright (c) 2025-2026 Andrea Ballestrazzi

add_executable(verso-example-basic "basic/main.cpp")
target_link_libraries(verso-example-basic PRIVATE verso)

add_executable(verso-example-comparisons "comparisons/main.cpp")
target_link_libraries(verso-example-comparisons PRIVATE verso)

if(UNIX)
    target_compile_options(verso-example-basic PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-comparisons PRIVATE -Wall -Wextra -Werror)
endif()
