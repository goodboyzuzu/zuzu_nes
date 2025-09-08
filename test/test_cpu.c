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

void test_CPU_write_read(void) {
    CPU_write(cpu, 0x1234,0xAB);
    TEST_ASSERT_EQUAL_UINT8(0xAB, CPU_read(cpu, 0x1234));
}

void test_CPU_set_get_flag(void) {
    CPU_set_flag(cpu, FLAG_BREAK, 1);
    TEST_ASSERT_EQUAL_UINT8(1, CPU_get_flag(cpu, FLAG_BREAK));
    CPU_set_flag(cpu, FLAG_BREAK, 0);
    TEST_ASSERT_EQUAL_UINT8(0, CPU_get_flag(cpu, FLAG_BREAK));
    TEST_ASSERT_EQUAL_UINT8(0x10, FLAG_BREAK);
}

void test_fetch(void) {
    /* set opcode to 2. addressing mode is IMP*/
    cpu->opcode = 0x02;
    cpu->addr_abs = 0x1234;
    CPU_write(cpu, 0x1234, 0x56);
    cpu->fetched = 0x78;
    fetch(cpu);
    TEST_ASSERT_EQUAL_UINT8(0x78, cpu->fetched); /* since IMP, fetched should be 0 */
    /* set opcode to 0. addressing mode: IMM*/
    cpu->opcode = 0x00;
    fetch(cpu);
    /* since IMM, fetched should be value at addr_abs */
    TEST_ASSERT_EQUAL_UINT8(0x56, cpu->fetched);
}

void test_CPU_reset(void){
    /*address of known state is stored in 0xFFFC*/
    CPU_write(cpu, 0xFFFC, 0x34);
    CPU_write(cpu, 0xFFFD, 0x12);
    /* set initial state of registers */
    cpu->a = 0xFF; cpu->x = 0xFF; cpu->y = 0xFF; cpu->sp = 0xFF; cpu->status = 0xFF;
    CPU_reset(cpu);
    TEST_ASSERT_EQUAL_UINT16(0X1234, cpu->pc);
    TEST_ASSERT_EQUAL_UINT8(0X00, cpu->a);
    TEST_ASSERT_EQUAL_UINT8(0X00, cpu->x);
    TEST_ASSERT_EQUAL_UINT8(0X00, cpu->y);
    TEST_ASSERT_EQUAL_UINT8(0XFD, cpu->sp); /*stack pointer will be 0xFD*/
    TEST_ASSERT_EQUAL_UINT8(0X00 | FLAG_UNUSED, cpu->status); /* status will be 0x20 */
    TEST_ASSERT_EQUAL_UINT16(0x00, cpu->addr_abs);
    TEST_ASSERT_EQUAL_UINT16(0x00, cpu->addr_rel);
    TEST_ASSERT_EQUAL_UINT8(8, cpu->cycles); /* reset takes 8 cycles */
}

void test_CPU_irq(void){
    /* ensure irq does not run when interrupt disable is 1*/
    cpu->sp = 0x88;/* set to known value. else sp=0x00, sp-1=0xFF*/
    CPU_write(cpu, 0xFFFE, 0x34); CPU_write(cpu, 0xFFFF, 0x12);

    cpu->pc = 0x1001; cpu->status = FLAG_INTERRUPT;
    CPU_irq(cpu);
    TEST_ASSERT_EQUAL_UINT16(0x1001, cpu->pc); /* pc should remain unchanged */

    /* set interrupt disable to 0*/
    cpu->status = FLAG_UNUSED;
    CPU_irq(cpu);
    /* pc change to updated value in 0xFE*/
    TEST_ASSERT_EQUAL_UINT16(0X1234, cpu->pc);

    /* read stack pointer of status, lo and hi addr*/
    cpu->status = (FLAG_UNUSED | FLAG_INTERRUPT) & (~FLAG_BREAK);
    TEST_ASSERT_EQUAL_UINT8(cpu->status,CPU_read(cpu,0x0100+cpu->sp+1));
    TEST_ASSERT_EQUAL_UINT8(0x01,CPU_read(cpu,0x0100+cpu->sp+2));
    TEST_ASSERT_EQUAL_UINT8(0x10,CPU_read(cpu,0x0100+cpu->sp+3));

    TEST_ASSERT_EQUAL_UINT8(7,cpu->cycles);
}

void test_CPU_nmi(void) {
    /* ensure irq does not run when interrupt disable is 1*/
    cpu->sp = 0x88;/* set to known value. else sp=0x00, sp-1=0xFF*/
    cpu->pc = 0x1001;
    CPU_write(cpu, 0xFFFA, 0x34); CPU_write(cpu, 0xFFFB, 0x12);

    CPU_nmi(cpu);

    /* pc change to updated value in 0xFA*/
    TEST_ASSERT_EQUAL_UINT16(0X1234, cpu->pc);
    /* read stack pointer of status, lo and hi addr*/
    cpu->status = (FLAG_UNUSED | FLAG_INTERRUPT) & (~FLAG_BREAK);
    TEST_ASSERT_EQUAL_UINT8(cpu->status,CPU_read(cpu,0x0100+cpu->sp+1));
    TEST_ASSERT_EQUAL_UINT8(0x01,CPU_read(cpu,0x0100+cpu->sp+2));
    TEST_ASSERT_EQUAL_UINT8(0x10,CPU_read(cpu,0x0100+cpu->sp+3));

    TEST_ASSERT_EQUAL_UINT8(8,cpu->cycles);
}

void test_CPU_clock(void) {
    /* when cycles!=0, it will not read opcode*/
    cpu->cycles = 7;
    CPU_clock(cpu);
    TEST_ASSERT_EQUAL_UINT8(6,cpu->cycles);

    /*
     * hard to implement, lazy to mock function too
     */
    cpu->cycles = 0;
    cpu->pc = 0x1001; CPU_write(cpu,0x1001,0x00);
    CPU_clock(cpu);
}

void test_CPU_complete(void) {
    cpu->cycles = 2;
    TEST_ASSERT_FALSE(CPU_complete(cpu));
    cpu->cycles = 0;
    TEST_ASSERT_TRUE(CPU_complete(cpu));
}

void test_BRK(void) {
    cpu->sp = 0x88;
    TEST_ASSERT_EQUAL_UINT8(cpu->status,CPU_read(cpu,0x0100+cpu->sp+1));
    TEST_ASSERT_EQUAL_UINT8(0x01,CPU_read(cpu,0x0100+cpu->sp+2));
    TEST_ASSERT_EQUAL_UINT8(0x10,CPU_read(cpu,0x0100+cpu->sp+3));
}

void test_ADC(void) {
    /* since it adds a + fetched */
    /*
     * MSB: 0+0=1 (overflow)
     */
    cpu->a = 0B01111111;cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B01010101); cpu->temp = 0B00000000;cpu->status=0;
    ADC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_NEGATIVE));

    /*
     * MSB: 1+1=0 (overflow)
     */
    cpu->a = 0B10000000; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B11010101); cpu->temp = 0B00000000;cpu->status=0;
    ADC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_NEGATIVE));

    /*
     * MSB: 0+1=1 (no overflow)
     */
    cpu->a = 0B01111111; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B11010101); cpu->temp = 0B00000000;cpu->status=0;
    ADC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_NEGATIVE));

    /*
     * MSB: 0+0=0 (no overflow)
     */
    cpu->a = 0B00000000; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B01010101); cpu->temp = 0B00000000;cpu->status=0;
    ADC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_NEGATIVE));
}

void test_SBC(void) {
    cpu->a = 0B01111111; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B01010101); cpu->temp = 0B00000000;cpu->status=0x21;
    /* a=127, fetched=85, carry=1. Result=42 */
    SBC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_NEGATIVE));

    cpu->a =0B10000001; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B00000010); cpu->temp = 0B00000000;cpu->status=0x21;
    /* a=-127, fetched= 2, carry=1*/
    SBC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_NEGATIVE));

    cpu->a =0B00000110; cpu->addr_abs=0x1234; CPU_write(cpu,0x1234,0B00001010); cpu->temp = 0B00000000;cpu->status=0x20;
    /* a=5, fetched = 10, carry =0*/
    SBC(cpu);
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_ZERO));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_CARRY));
    TEST_ASSERT_FALSE( CPU_get_flag(cpu, FLAG_OVERFLOW));
    TEST_ASSERT_TRUE( CPU_get_flag(cpu, FLAG_NEGATIVE));
}

void test_CPU_run_all_tests( void) {
    RUN_TEST( test_CPU_initial_state);
    RUN_TEST( test_CPU_write_read);
    RUN_TEST( test_fetch);
    RUN_TEST( test_CPU_reset);
    RUN_TEST( test_CPU_set_get_flag);
    RUN_TEST( test_CPU_irq);
    RUN_TEST( test_CPU_nmi);
    RUN_TEST( test_CPU_clock);
    RUN_TEST( test_CPU_complete);
    RUN_TEST( test_ADC);
    RUN_TEST( test_SBC);
}