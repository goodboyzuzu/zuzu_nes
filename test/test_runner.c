#include <stdio.h>
#include "unity.h"

#include "test_bus.h"
#include "test_cpu.h"

Bus* bus = NULL;
CPU* cpu = NULL;

void setUp(void){
    bus = Bus_init();
    cpu = bus->cpu;
};
void tearDown(void){
    Bus_destroy(&bus);
};

int main(void) {
    UNITY_BEGIN();

    test_Bus_run_all_tests();
    test_CPU_run_all_tests();

    return UNITY_END();
}