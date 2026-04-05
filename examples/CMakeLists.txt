# Copyright (c) 2025-2026 Andrea Ballestrazzi

add_executable(verso-example-basic "basic/main.cpp")
target_link_libraries(verso-example-basic PRIVATE verso)

add_executable(verso-example-comparisons "comparisons/main.cpp")
target_link_libraries(verso-example-comparisons PRIVATE verso)

add_executable(verso-example-from-string "from-string/main.cpp")
target_link_libraries(verso-example-from-string PRIVATE verso)

if(UNIX)
    target_compile_options(verso-example-basic PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-comparisons PRIVATE -Wall -Wextra -Werror)
    target_compile_options(verso-example-from-string PRIVATE -Wall -Wextra -Werror)
endif()
