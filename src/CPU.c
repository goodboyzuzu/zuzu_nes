#include "CPU.h"
#include <stdlib.h>
#include <string.h>

 static const INSTRUCTION instruction_set[] = {
        { "BRK", &BRK, &IMM, 7 },{ "ORA", &ORA, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ORA", &ORA, &ZP0, 3 },{ "ASL", &ASL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHP", &PHP, &IMP, 3 },{ "ORA", &ORA, &IMM, 2 },{ "ASL", &ASL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABS, 4 },{ "ASL", &ASL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BPL", &BPL, &REL, 2 },{ "ORA", &ORA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ZPX, 4 },{ "ASL", &ASL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLC", &CLC, &IMP, 2 },{ "ORA", &ORA, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABX, 4 },{ "ASL", &ASL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "JSR", &JSR, &ABS, 6 },{ "AND", &AND, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "BIT", &BIT, &ZP0, 3 },{ "AND", &AND, &ZP0, 3 },{ "ROL", &ROL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLP", &PLP, &IMP, 4 },{ "AND", &AND, &IMM, 2 },{ "ROL", &ROL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "BIT", &BIT, &ABS, 4 },{ "AND", &AND, &ABS, 4 },{ "ROL", &ROL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BMI", &BMI, &REL, 2 },{ "AND", &AND, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ZPX, 4 },{ "ROL", &ROL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEC", &SEC, &IMP, 2 },{ "AND", &AND, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ABX, 4 },{ "ROL", &ROL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "RTI", &RTI, &IMP, 6 },{ "EOR", &EOR, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "EOR", &EOR, &ZP0, 3 },{ "LSR", &LSR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHA", &PHA, &IMP, 3 },{ "EOR", &EOR, &IMM, 2 },{ "LSR", &LSR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &ABS, 3 },{ "EOR", &EOR, &ABS, 4 },{ "LSR", &LSR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BVC", &BVC, &REL, 2 },{ "EOR", &EOR, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ZPX, 4 },{ "LSR", &LSR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLI", &CLI, &IMP, 2 },{ "EOR", &EOR, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ABX, 4 },{ "LSR", &LSR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "RTS", &RTS, &IMP, 6 },{ "ADC", &ADC, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ADC", &ADC, &ZP0, 3 },{ "ROR", &ROR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLA", &PLA, &IMP, 4 },{ "ADC", &ADC, &IMM, 2 },{ "ROR", &ROR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &IND, 5 },{ "ADC", &ADC, &ABS, 4 },{ "ROR", &ROR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BVS", &BVS, &REL, 2 },{ "ADC", &ADC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ZPX, 4 },{ "ROR", &ROR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEI", &SEI, &IMP, 2 },{ "ADC", &ADC, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ABX, 4 },{ "ROR", &ROR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "???", &NOP, &IMP, 2 },{ "STA", &STA, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZP0, 3 },{ "STA", &STA, &ZP0, 3 },{ "STX", &STX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "DEY", &DEY, &IMP, 2 },{ "???", &NOP, &IMP, 2 },{ "TXA", &TXA, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "STY", &STY, &ABS, 4 },{ "STA", &STA, &ABS, 4 },{ "STX", &STX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
		{ "BCC", &BCC, &REL, 2 },{ "STA", &STA, &IZY, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZPX, 4 },{ "STA", &STA, &ZPX, 4 },{ "STX", &STX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "TYA", &TYA, &IMP, 2 },{ "STA", &STA, &ABY, 5 },{ "TXS", &TXS, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "???", &NOP, &IMP, 5 },{ "STA", &STA, &ABX, 5 },{ "???", &XXX, &IMP, 5 },{ "???", &XXX, &IMP, 5 },
		{ "LDY", &LDY, &IMM, 2 },{ "LDA", &LDA, &IZX, 6 },{ "LDX", &LDX, &IMM, 2 },{ "???", &XXX, &IMP, 6 },{ "LDY", &LDY, &ZP0, 3 },{ "LDA", &LDA, &ZP0, 3 },{ "LDX", &LDX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "TAY", &TAY, &IMP, 2 },{ "LDA", &LDA, &IMM, 2 },{ "TAX", &TAX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "LDY", &LDY, &ABS, 4 },{ "LDA", &LDA, &ABS, 4 },{ "LDX", &LDX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
		{ "BCS", &BCS, &REL, 2 },{ "LDA", &LDA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "LDY", &LDY, &ZPX, 4 },{ "LDA", &LDA, &ZPX, 4 },{ "LDX", &LDX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "CLV", &CLV, &IMP, 2 },{ "LDA", &LDA, &ABY, 4 },{ "TSX", &TSX, &IMP, 2 },{ "???", &XXX, &IMP, 4 },{ "LDY", &LDY, &ABX, 4 },{ "LDA", &LDA, &ABX, 4 },{ "LDX", &LDX, &ABY, 4 },{ "???", &XXX, &IMP, 4 },
		{ "CPY", &CPY, &IMM, 2 },{ "CMP", &CMP, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPY", &CPY, &ZP0, 3 },{ "CMP", &CMP, &ZP0, 3 },{ "DEC", &DEC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INY", &INY, &IMP, 2 },{ "CMP", &CMP, &IMM, 2 },{ "DEX", &DEX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "CPY", &CPY, &ABS, 4 },{ "CMP", &CMP, &ABS, 4 },{ "DEC", &DEC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BNE", &BNE, &REL, 2 },{ "CMP", &CMP, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ZPX, 4 },{ "DEC", &DEC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLD", &CLD, &IMP, 2 },{ "CMP", &CMP, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ABX, 4 },{ "DEC", &DEC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
		{ "CPX", &CPX, &IMM, 2 },{ "SBC", &SBC, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPX", &CPX, &ZP0, 3 },{ "SBC", &SBC, &ZP0, 3 },{ "INC", &INC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INX", &INX, &IMP, 2 },{ "SBC", &SBC, &IMM, 2 },{ "NOP", &NOP, &IMP, 2 },{ "???", &SBC, &IMP, 2 },{ "CPX", &CPX, &ABS, 4 },{ "SBC", &SBC, &ABS, 4 },{ "INC", &INC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
		{ "BEQ", &BEQ, &REL, 2 },{ "SBC", &SBC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ZPX, 4 },{ "INC", &INC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SED", &SED, &IMP, 2 },{ "SBC", &SBC, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ABX, 4 },{ "INC", &INC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
};

CPU* CPU_init(){
    debug_printf("CPU initialized;");
    CPU* cpu = malloc(sizeof(CPU));
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->sp = 0x00;
    cpu->pc = 0x0000;
    cpu->status = 0x00;

    cpu->lookup_table = malloc(sizeof(instruction_set));
    memcpy(cpu->lookup_table, instruction_set, sizeof(instruction_set));
    return cpu;
}
void CPU_destroy(CPU** cpu){
    if (cpu && *cpu) {
        free((*cpu)->lookup_table);
        free(*cpu);
        debug_printf("CPU_destroyed;");
    }
}

void CPU_connect_Bus(CPU* cpu, Bus* bus){
    cpu->bus = bus;
}

uint8_t CPU_read(CPU* cpu, uint16_t a){
    return Bus_read(cpu->bus, a, false);
}

void CPU_write(CPU* cpu, uint16_t a, uint8_t d){
    Bus_write(cpu->bus, a, d);
}

uint8_t fetch(CPU* cpu){
    if (!(cpu->lookup_table[cpu->opcode].addrmode == &IMP)){
        cpu->fetched = CPU_read(cpu, cpu->addr_abs);
    }
    return cpu->fetched;
}

bool CPU_get_flag(CPU* cpu, enum CPU_FLAGS f) {
    return ((cpu->status & f) > 0) ? 1 : 0;
}

void CPU_set_flag(CPU* cpu, enum CPU_FLAGS f, bool v) {
    if (v)
        cpu->status |= f;
    else
        cpu->status &= ~f;
}

/* external event function */

void CPU_reset(CPU* cpu){
    cpu->addr_abs = 0xFFFC;
    uint16_t lo = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t hi = CPU_read(cpu, cpu->addr_abs + 1);

    cpu->pc = (hi << 8) | lo;

    /* reset internal registers */
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0xFD;
    /* status either unused flag enum CPU_flags u */
    cpu->status = 0x00 | FLAG_UNUSED; 

    cpu->addr_rel = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->fetched = 0x00;

    cpu->cycles = 8;
}

void CPU_irq(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_INTERRUPT) == 0){
        CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00FF);
        cpu->sp--;
        CPU_write(cpu, 0x0100 + cpu->sp, cpu->pc & 0x00FF);
        cpu->sp--;

        CPU_set_flag(cpu, FLAG_BREAK, 0);
        CPU_set_flag(cpu, FLAG_UNUSED, 1);
        CPU_set_flag(cpu, FLAG_INTERRUPT, 1);
        CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
        cpu->sp--;

        cpu->addr_abs = 0xFFFE;
        uint16_t lo = CPU_read(cpu, cpu->addr_abs + 0);
        uint16_t hi = CPU_read(cpu, cpu->addr_abs + 1);
        cpu->pc = (hi << 8) | lo;

        cpu->cycles = 7;
    }
}

void CPU_nmi(CPU* cpu){
    CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00FF);
    cpu->sp--;
    CPU_write(cpu, 0x0100 + cpu->sp, cpu->pc & 0x00FF);
    cpu->sp--;

    CPU_set_flag(cpu, FLAG_BREAK, 0);
    CPU_set_flag(cpu, FLAG_UNUSED, 1);
    CPU_set_flag(cpu, FLAG_INTERRUPT, 1);
    CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
    cpu->sp--;

    cpu->addr_abs = 0xFFFA;
    uint16_t lo = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t hi = CPU_read(cpu, cpu->addr_abs + 1);
    cpu->pc = (hi << 8) | lo;

    cpu->cycles = 8;
}

void CPU_clock(CPU* cpu){
    if (cpu->cycles == 0){
        cpu->opcode = CPU_read(cpu, cpu->pc);
        CPU_set_flag(cpu, FLAG_UNUSED, 1);
        cpu->pc++;

        cpu->cycles = cpu->lookup_table[cpu->opcode].instruction_cycles;

        uint8_t additional_cycle1 = cpu->lookup_table[cpu->opcode].addrmode(cpu);
        uint8_t additional_cycle2 = cpu->lookup_table[cpu->opcode].operate(cpu);

        cpu->cycles += (additional_cycle1 & additional_cycle2);
        CPU_set_flag(cpu, FLAG_UNUSED, 1);
    }
    cpu->clock_count++;
    cpu->cycles--;
}

bool CPU_complete(CPU* cpu){
    return cpu->cycles == 0;
}

/* addressing mode */

/* does nothing, for instruction that does simple stuff */
uint8_t IMP(CPU* cpu){
    cpu->fetched = cpu->a;
    return 0;
}

/* after storing pc++ to addr_abs, what does it do after? */
uint8_t IMM(CPU* cpu){
    cpu->addr_abs = cpu->pc++;
    return 0;
}

/* No need load twice as addr x2 of data*/
uint8_t ZP0(CPU* cpu){
    cpu->addr_abs = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    cpu->addr_abs &= 0x00FF;
    return 0;
}

/* zero page addressing + x register */
uint8_t ZPX(CPU* cpu){
    cpu->addr_abs = (CPU_read(cpu, cpu->pc) + cpu->x) & 0x00FF;
    cpu->pc++;
    return 0;
}

/* zero page addressing + y register */
uint8_t ZPY(CPU* cpu){
    cpu->addr_abs = (CPU_read(cpu, cpu->pc) + cpu->y) & 0x00FF;
    cpu->pc++;
    return 0;
}

uint8_t REL(CPU* cpu){
    cpu->addr_rel = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    if (cpu->addr_rel & 0x80)
        cpu->addr_rel |= 0xFF00;
    return 0;
}

uint8_t ABS(CPU* cpu){
    uint16_t lo = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t ABX(CPU* cpu){
    uint16_t lo = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = ((hi << 8) | lo) + cpu->x;

    if ((cpu->addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t ABY(CPU* cpu){
    uint16_t lo = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = ((hi << 8) | lo) + cpu->y;

    if ((cpu->addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

/* indirect: read from ptr addr, if lower byte of ptr addr is 0xff, then top page stays the same. Bug */
uint8_t IND(CPU* cpu){
    uint16_t ptr_lo = CPU_read(cpu, cpu->pc);
    cpu->pc++;
    uint16_t ptr_hi = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if (ptr_lo == 0x00FF){ /* Simulate page boundary hardware bug */
        cpu->addr_abs = (CPU_read(cpu, ptr & 0xFF00) << 8) | CPU_read(cpu, ptr + 0);
    }
    else{
        cpu->addr_abs = (CPU_read(cpu, ptr + 1) << 8) | CPU_read(cpu, ptr + 0);
    }
    return 0;
}

/* indirect + x register */
uint8_t IZX(CPU* cpu){
    uint16_t t = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = CPU_read(cpu, (t + (uint16_t)cpu->x) & 0x00FF);
    uint16_t hi = CPU_read(cpu, (t + (uint16_t)cpu->x + 1) & 0x00FF);

    cpu->addr_abs = (hi << 8) | lo;
    return 0;
}

/* indexes location in page 0x00 */
uint8_t IZY(CPU* cpu){
    uint16_t t = CPU_read(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = CPU_read(cpu, t & 0x00FF);
    uint16_t hi = CPU_read(cpu, (t + 1) & 0x00FF);

    cpu->addr_abs = ((hi << 8) | lo) + cpu->y;

    if ((cpu->addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

/* instruction */

/* illegal opcode */
uint8_t XXX(CPU* cpu){
    return 0;
}

uint8_t ADC(CPU* cpu){
    fetch(cpu);
    uint16_t temp = (uint16_t)cpu->a + (uint16_t)cpu->fetched + (uint16_t)CPU_get_flag(cpu, FLAG_CARRY);
    CPU_set_flag(cpu, FLAG_CARRY, temp > 255);
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0);
    CPU_set_flag(cpu, FLAG_OVERFLOW, (~((uint16_t)cpu->a ^ (uint16_t)cpu->fetched) & ((uint16_t)cpu->a ^ (uint16_t)temp)) & 0x0080);
    CPU_set_flag(cpu, FLAG_NEGATIVE, temp & 0x80);
    cpu->a = temp & 0x00FF;
    return 1;
}

uint8_t SBC(CPU* cpu){
    fetch(cpu);
    uint16_t value = ((uint16_t)cpu->fetched) ^ 0x00FF;
    uint16_t temp = (uint16_t)cpu->a + value + (uint16_t)CPU_get_flag(cpu, FLAG_CARRY);
    CPU_set_flag(cpu, FLAG_CARRY, temp & 0xFF00);
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0);
    CPU_set_flag(cpu, FLAG_OVERFLOW, (temp ^ (uint16_t)cpu->a) & (temp ^ value) & 0x0080);
    CPU_set_flag(cpu, FLAG_NEGATIVE, temp & 0x80);
    cpu->a = temp & 0x00FF;
    return 1;
}

uint8_t AND(CPU* cpu){
    fetch(cpu);
    cpu->a = cpu->a & cpu->fetched;
    CPU_set_flag(cpu, FLAG_ZERO, cpu->a == 0x00);
    CPU_set_flag(cpu, FLAG_NEGATIVE, cpu->a & 0x80);
    return 1;
}

uint8_t ASL(CPU* cpu){
    fetch(cpu);
    uint16_t temp = (uint16_t)cpu->fetched << 1;
    CPU_set_flag(cpu, FLAG_CARRY, (temp & 0xFF00) > 0);
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0x00);
    CPU_set_flag(cpu, FLAG_NEGATIVE, temp & 0x80);
    if (cpu->lookup_table[cpu->opcode].addrmode == &IMP){
        cpu->a = temp & 0x00FF;
    }
    else{
        CPU_write(cpu, cpu->addr_abs, temp & 0x00FF);
    }
    return 0;
}

/* branch if carry clear */
uint8_t BCC(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_CARRY) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* branch if carry set */
uint8_t BCS(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_CARRY) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* branch if equal */
uint8_t BEQ(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_ZERO) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* bit test */
uint8_t BIT(CPU* cpu){
    fetch(cpu);
    uint8_t temp = cpu->a & cpu->fetched;
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0x00);
    CPU_set_flag(cpu, FLAG_OVERFLOW, cpu->fetched & 0x40);
    CPU_set_flag(cpu, FLAG_NEGATIVE, cpu->fetched & 0x80);
    return 0;
}

/* branch if negative */
uint8_t BMI(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_NEGATIVE) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* branch if not equal */
uint8_t BNE(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_ZERO) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* branch if positive */
uint8_t BPL(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_NEGATIVE) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

/* break */
uint8_t BRK(CPU* cpu){
    cpu->pc++;

    CPU_set_flag(cpu, FLAG_INTERRUPT, 1);
    CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00FF);
    cpu->sp--;
    CPU_write(cpu, 0x0100 + cpu->sp, cpu->pc & 0x00FF);
    cpu->sp--;

    CPU_set_flag(cpu, FLAG_BREAK, 1);
    CPU_set_flag(cpu, FLAG_UNUSED, 1);
    CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
    cpu->sp--;

    cpu->addr_abs = 0xFFFE;
    uint16_t lo = CPU_read(cpu, cpu->addr_abs + 0);
    uint16_t hi = CPU_read(cpu, cpu->addr_abs + 1);
    cpu->pc = (hi << 8) | lo;
    return 0;
}

uint8_t BVC(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_OVERFLOW) == 0){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BVS(CPU* cpu){
    if (CPU_get_flag(cpu, FLAG_OVERFLOW) == 1){
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
            cpu->cycles++;

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t CLC(CPU* cpu){
    CPU_set_flag(cpu, FLAG_CARRY, 0);
    return 0;
}

uint8_t CLD(CPU* cpu){
    CPU_set_flag(cpu, FLAG_DECIMAL, 0);
    return 0;
}

uint8_t CLI(CPU* cpu){
    CPU_set_flag(cpu, FLAG_INTERRUPT, 0);
    return 0;
}

uint8_t CLV(CPU* cpu){
    CPU_set_flag(cpu, FLAG_OVERFLOW, 0);
    return 0;
}

uint8_t CMP(CPU* cpu){
    fetch(cpu);
    uint16_t temp = (uint16_t)cpu->a - (uint16_t)cpu->fetched;
    CPU_set_flag(cpu, FLAG_CARRY, cpu->a >= cpu->fetched);
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0x0000);
    CPU_set_flag(cpu, FLAG_NEGATIVE, temp & 0x0080);
    return 1;
}

uint8_t CPX(CPU* cpu){
    fetch(cpu);
    uint16_t temp = (uint16_t)cpu->x - (uint16_t)cpu->fetched;
    CPU_set_flag(cpu, FLAG_CARRY, cpu->x >= cpu->fetched);
    CPU_set_flag(cpu, FLAG_ZERO, (temp & 0x00FF) == 0x0000);
    CPU_set_flag(cpu, FLAG_NEGATIVE, temp & 0x0080);
    return 1;
}