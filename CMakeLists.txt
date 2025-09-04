cmake_minimum_required(VERSION 3.10)
project(nes_emulator C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

include_directories(helper include)

add_executable(nes_emulator
    helper/debug_printf.c
    src/main.c
    src/Bus.c
    src/CPU.c
)

add_executable(test_output
    helper/debug_printf.c
    test/test_runner.c
    test/test_bus.c
    test/test_cpu.c
    test/unity.c
    src/Bus.c
    src/CPU.c
)

target_include_directories(test_output PRIVATE include test)