#include "unity.h"
#include "CPU.h"

extern Bus* bus;
extern CPU* cpu;

void test_CPU_initial_state(void) {
    TEST_ASSERT_EQUAL_UINT16(0x0000, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->x);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->y);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->status);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->sp);
}

void test_CPU_reset(void){
    cpu->pc = 0x1234;
    cpu->a = 0x56;
    cpu->x = 0x78;
    cpu->y = 0x9A;
    cpu->status = 0xBC;
    cpu->sp = 0xDE;

    CPU_reset(cpu);

    TEST_ASSERT_EQUAL_UINT16(0x0000, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->a);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->x);
    TEST_ASSERT_EQUAL_UINT8(0x00, cpu->y);
    TEST_ASSERT_EQUAL_UINT8(0x20, cpu->status);
    TEST_ASSERT_EQUAL_UINT8(0xFD, cpu->sp);
    TEST_ASSERT_EQUAL_UINT8(8, cpu->cycles);
}


void test_CPU_set_get_flag(void) {
    CPU_set_flag(cpu, FLAG_BREAK, 1);
    TEST_ASSERT_EQUAL_UINT8(1, CPU_get_flag(cpu, FLAG_BREAK));
    CPU_set_flag(cpu, FLAG_BREAK, 0);
    TEST_ASSERT_EQUAL_UINT8(0, CPU_get_flag(cpu, FLAG_BREAK));
    TEST_ASSERT_EQUAL_UINT8(0x10, FLAG_BREAK);
}

void test_CPU_irq(void){
    cpu->pc = 0x1234;
    cpu->sp = 0xFF;
    cpu->status = 0x00;

    /* Set the interrupt vector to 0xABCD */
    Bus_write(bus, 0xFFFE, 0xCD);
    Bus_write(bus, 0xFFFF, 0xAB);

    CPU_irq(cpu);

    TEST_ASSERT_EQUAL_UINT16(0xABCD, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0xFC, cpu->sp); /* Stack pointer should decrement by 3 */
    TEST_ASSERT_TRUE(CPU_get_flag(cpu, FLAG_INTERRUPT)); /* Interrupt flag should be set */
    TEST_ASSERT_FALSE(CPU_get_flag(cpu, FLAG_BREAK)); /* Break flag should be clear */
    TEST_ASSERT_TRUE(CPU_get_flag(cpu, FLAG_UNUSED)); /* Unused flag should be set */
}

void test_CPU_nmi(void){
    cpu->pc = 0x1234;
    cpu->sp = 0xFF;
    cpu->status = 0x00;

    /* Set the NMI vector to 0xABCD */
    Bus_write(bus, 0xFFFA, 0xCD);
    Bus_write(bus, 0xFFFB, 0xAB);

    CPU_nmi(cpu);

    TEST_ASSERT_EQUAL_UINT16(0xABCD, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0xFC, cpu->sp); /* Stack pointer should decrement by 3 */
    TEST_ASSERT_TRUE(CPU_get_flag(cpu, FLAG_INTERRUPT)); /* Interrupt flag should be set */
    TEST_ASSERT_FALSE(CPU_get_flag(cpu, FLAG_BREAK)); /* Break flag should be set */
    TEST_ASSERT_TRUE(CPU_get_flag(cpu, FLAG_UNUSED)); /* Unused flag should be set */
}

void test_CPU_clock(void){
    /* when clock = 0, opcode=0 */
    cpu->pc = 0x0000;
    cpu->cycles = 0;
    cpu->opcode = 0x00; /* BRK instruction */
    /* print out all values in lookup */
    INSTRUCTION zuzu = cpu->lookup_table[cpu->opcode];
    printf("Opcode: 0x%02X, Name: %s, Cycles: %d\n", cpu->opcode, zuzu.name, zuzu.instruction_cycles);
}


void test_CPU_run_all_tests(void) {
    RUN_TEST(test_CPU_reset);
    RUN_TEST(test_CPU_set_get_flag);
    RUN_TEST(test_CPU_irq);
    RUN_TEST(test_CPU_nmi);
    RUN_TEST(test_CPU_clock);
}