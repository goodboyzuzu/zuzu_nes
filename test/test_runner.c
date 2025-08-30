#include <stdio.h>
#include "unity.h"

#include "test_bus.c"
#include "test_cpu.c"

int main(void) {
    UNITY_BEGIN();

    test_bus_run_all_tests();
    // test_cpu_run_all_tests();

    return UNITY_END();
}