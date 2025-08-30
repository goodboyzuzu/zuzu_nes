#include "unity.h"
#include "Bus.h"
#include "CPU.h"

static Bus* bus = NULL;

void bus_global_setup(void) {
    bus = Bus_init();
}

void bus_global_teardown(void) {
    Bus_destroy(&bus);
}

void setUp(void) {
}

void tearDown(void) {
}

void test_Bus_initial_state(void) {
    // Check that RAM is zeroed
    TEST_ASSERT_EACH_EQUAL_UINT8(0, bus->ram, 2048);
}

void test_Bus_write_and_read_ram(void) {
    uint16_t addr = 0x000A;
    uint8_t value = 0x42;
    Bus_write(bus,addr,value);
    uint8_t read = Bus_read(bus,addr,1);
    TEST_ASSERT_EQUAL_UINT8(value, read);
}


// test runner interface
void test_bus_run_all_tests(void) {
    bus_global_setup();
    RUN_TEST(test_Bus_initial_state);
    RUN_TEST(test_Bus_write_and_read_ram);
    bus_global_teardown();
}
