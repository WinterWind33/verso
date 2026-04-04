# Copyright (c) 2025-2026 Andrea Ballestrazzi

add_executable(verso-example-basic "basic/main.cpp")
target_link_libraries(verso-example-basic PRIVATE verso)

if(UNIX)
    target_compile_options(verso-example-basic PRIVATE -Wall -Wextra -Werror)
endif()
