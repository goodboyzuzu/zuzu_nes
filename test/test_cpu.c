#include "unity.h"
#include "CPU.h"

static CPU* cpu = NULL;

void cpu_global_setup(void) {
    cpu = CPU_init();
}

void cpu_global_teardown(void) {
    CPU_destroy(&cpu);
}


void test_CPU_initial_state(void) {
    TEST_ASSERT_EQUAL_UINT16(0x0000, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->x);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->y);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->status);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->sp);
}

void test_CPU_all_tests(void) {
    cpu_global_setup();
    RUN_TEST(test_CPU_initial_state);
    cpu_global_teardown();
}