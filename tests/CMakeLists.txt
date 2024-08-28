cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tests
    main.c
    test_vector.c
)

target_include_directories(tests PRIVATE
    ../include
)

target_link_libraries(tests PRIVATE
    rendertest
)

add_test(
    NAME tests
    COMMAND tests
)