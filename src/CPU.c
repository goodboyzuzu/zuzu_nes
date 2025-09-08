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


bool CPU_get_flag(CPU* cpu, enum CPU_FLAGS f) {
	return ((cpu->status & f) > 0) ? 1 : 0;
}

void CPU_set_flag(CPU* cpu, enum CPU_FLAGS f, bool v) {
	if (v)
		cpu->status |= f;
	else
		cpu->status &= ~f;
}


uint8_t fetch(CPU* cpu) {
	if (cpu->lookup_table[cpu->opcode].addrmode != &IMP)
		cpu->fetched = CPU_read(cpu, cpu->addr_abs);
	return cpu->fetched;
}

/*
 * external event functions
 */

void CPU_reset(CPU *cpu) {
	/* reset goes to known address stored in addr_abs*/
	cpu->addr_abs = 0xFFFC;
	const uint16_t lo = CPU_read(cpu, cpu->addr_abs + 0);
	const uint16_t hi = CPU_read(cpu, cpu->addr_abs + 1);
	cpu->pc = (hi<<8) | lo;
	/*clear internal registers*/
	cpu->a = 0; cpu->x = 0; cpu->y = 0; cpu->sp = 0xFD; cpu->status = 0x00 | FLAG_UNUSED;
	/* clear helper functions*/
	cpu->addr_rel = 0x0000; cpu->addr_abs = 0x0000; cpu->fetched = 0x00;
	/* reset takes time */
	cpu->cycles = 8;
}

void CPU_irq(CPU *cpu) {
	/*interrupt request
	 * happens when interupt disable flag is 0
	 * set flag for interrupt disable=1 and break=0
	 * complete current instruction, then push pc and status to stack
	 * set pc to address stored at 0xFFFE (low byte) and 0xFFFF (high byte)
	 */
	if (CPU_get_flag(cpu, FLAG_INTERRUPT) == 0) {
		/* push pc to stack pointer*/
		CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00FF);
		cpu->sp--;
		CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc) & 0x00FF);
		cpu->sp--;

		/* set break=0, unused=1, interrupt=1 */
		CPU_set_flag(cpu, FLAG_BREAK, 0); CPU_set_flag(cpu, FLAG_UNUSED, 1); CPU_set_flag(cpu, FLAG_INTERRUPT, 1);
		/* write status to stack pointer */
		CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
		cpu->sp--;

		/* change pc from 0xfffe*/
		cpu->addr_abs = 0xFFFE;
		uint16_t lo = CPU_read(cpu, cpu->addr_abs+0);
		uint16_t hi = CPU_read(cpu, cpu->addr_abs+1);
		cpu->pc = hi<<8 | lo;
		cpu->cycles = 7;
	}

}

void CPU_nmi(CPU *cpu) {
	/*
	 * Non-maskable interrupt cannot be ignored, same as IRQ
	 * Read PC address from 0xFFFA
	 */
	CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc >> 8) & 0x00FF);
	cpu->sp--;
	CPU_write(cpu, 0x0100 + cpu->sp, (cpu->pc) & 0x00FF);
	cpu->sp--;

	/* set break=0, unused=1, interrupt=1 */
	CPU_set_flag(cpu, FLAG_BREAK, 0); CPU_set_flag(cpu, FLAG_UNUSED, 1); CPU_set_flag(cpu, FLAG_INTERRUPT, 1);
	/* write status to stack pointer */
	CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
	cpu->sp--;

	/* change pc from 0xfffe*/
	cpu->addr_abs = 0xFFFA;
	uint16_t lo = CPU_read(cpu, cpu->addr_abs+0);
	uint16_t hi = CPU_read(cpu, cpu->addr_abs+1);
	cpu->pc = hi<<8 | lo;
	cpu->cycles = 8;

}

void CPU_clock(CPU *cpu) {
	if (cpu->cycles == 0) {
		cpu->opcode = CPU_read(cpu, cpu->pc);
		CPU_set_flag(cpu, FLAG_UNUSED,1);
		cpu->pc++;
		cpu->cycles = cpu->lookup_table[cpu->opcode].instruction_cycles;
		uint8_t additional_cycle1 = (*cpu->lookup_table[cpu->opcode].addrmode)(cpu);
		uint8_t additional_cycle2 = (*cpu->lookup_table[cpu->opcode].operate)(cpu);
		cpu->cycles += additional_cycle1 + additional_cycle2;

	}
	cpu->clock_count++;
	cpu->cycles--;
}

bool CPU_complete(CPU *cpu) {
	return cpu->cycles == 0;
}

int CPU_disassemble(CPU *cpu, uint16_t start, uint16_t end) {
	return 0;
}

uint8_t ADC(CPU *cpu) {
	/*
	 * add with carry in
	 * overflow: look at MSB. P+P->N, N+N->P
	 * if A+M->R, overflow: A and M are same & A and R are different
	 *
	 */
	fetch(cpu);
	cpu->temp = cpu->a + cpu->fetched + CPU_get_flag(cpu,FLAG_CARRY);
	CPU_set_flag(cpu,FLAG_CARRY,cpu->temp>255);
	CPU_set_flag(cpu, FLAG_ZERO,cpu->temp == 0);
	CPU_set_flag(cpu, FLAG_OVERFLOW,~(cpu->a^cpu->fetched) & (cpu->a^cpu->temp) & 0x0080);
	CPU_set_flag(cpu, FLAG_NEGATIVE,cpu->temp & 0x80);
	cpu->a = cpu->temp | 0x00FF;
	return 1;
}

uint8_t AND(CPU *cpu) {
	/* bitwise logic and*/
	fetch(cpu);
	cpu->a = cpu->a & cpu->fetched;
	CPU_set_flag(cpu, FLAG_ZERO, cpu->a == 0x00);
	CPU_set_flag(cpu, FLAG_NEGATIVE, cpu->a & 0x80);
	return 1;
}

uint8_t ASL(CPU *cpu) {
	/*arithmetic shift left. put 0 for empty positions*/
	fetch(cpu);
	cpu->temp = (uint16_t)cpu->fetched << 1;
	CPU_set_flag(cpu, FLAG_CARRY, (cpu->temp & 0xFF00) > 0);
	CPU_set_flag(cpu, FLAG_ZERO, (cpu->temp & 0x00FF) == 0x00);
	CPU_set_flag(cpu, FLAG_NEGATIVE, cpu->temp & 0x80);
	if (cpu->lookup_table[cpu->opcode].addrmode == &IMP)
		cpu->a = cpu->temp & 0x00FF;
	else
		CPU_write(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
	return 0;
}

uint8_t BCC(CPU *cpu) {
	/* branch if carry clear */
	if (CPU_get_flag(cpu, FLAG_CARRY) == 0) {
		cpu->cycles++;
		cpu->addr_abs = cpu->pc + cpu->addr_rel;

		if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;

		cpu->pc = cpu->addr_abs;
	}
	return 0;
}

uint8_t BCS(CPU *cpu) {
	/* branch if carry set */
	if (CPU_get_flag(cpu, FLAG_CARRY) == 1) {
		cpu->cycles++;
		cpu->addr_abs = cpu->pc + cpu->addr_rel;

		if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;

		cpu->pc = cpu->addr_abs;
	}
	return 0;
}

uint8_t BEQ(CPU *cpu) {
	/* branch if equal */
	if (CPU_get_flag(cpu, FLAG_ZERO) == 1) {
		cpu->cycles++;
		cpu->addr_abs = cpu->pc + cpu->addr_rel;

		if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00))
			cpu->cycles++;

		cpu->pc = cpu->addr_abs;
	}
	return 0;
}

uint8_t BIT(CPU *cpu) {
}

uint8_t BMI(CPU *cpu) {
}

uint8_t BNE(CPU *cpu) {
}

uint8_t BPL(CPU *cpu) {
}

uint8_t BRK(CPU *cpu) {

	cpu->pc++;
	CPU_set_flag(cpu,FLAG_INTERRUPT,1); CPU_set_flag(cpu, FLAG_BREAK,1);
	CPU_write(cpu, 0x0100 + cpu->sp, cpu->pc >> 8 | 0x00FF);
	cpu->sp--;
	CPU_write(cpu, 0x0100 + cpu->sp, cpu->pc | 0x00FF);
	cpu->sp--;
	CPU_write(cpu, 0x0100 + cpu->sp, cpu->status);
	cpu->sp--;

	cpu->pc = CPU_read(cpu, 0xFFFE) | CPU_read(cpu, 0xFFFF) << 8;

	return 0;
}

uint8_t BVC(CPU *cpu) {
}

uint8_t BVS(CPU *cpu) {
}

uint8_t CLC(CPU *cpu) {
}

uint8_t CLD(CPU *cpu) {
}

uint8_t CLI(CPU *cpu) {
}

uint8_t CLV(CPU *cpu) {
}

uint8_t CMP(CPU *cpu) {
}

uint8_t CPX(CPU *cpu) {
}

uint8_t CPY(CPU *cpu) {
}

uint8_t DEC(CPU *cpu) {
}

uint8_t DEX(CPU *cpu) {
}

uint8_t DEY(CPU *cpu) {
}

uint8_t EOR(CPU *cpu) {
}

uint8_t INC(CPU *cpu) {
}

uint8_t INX(CPU *cpu) {
}

uint8_t INY(CPU *cpu) {
}

uint8_t JMP(CPU *cpu) {
}

uint8_t JSR(CPU *cpu) {
}

uint8_t LDA(CPU *cpu) {
}

uint8_t LDX(CPU *cpu) {
}

uint8_t LDY(CPU *cpu) {
}

uint8_t LSR(CPU *cpu) {
}

uint8_t NOP(CPU *cpu) {
}

uint8_t ORA(CPU *cpu) {
}

uint8_t PHA(CPU *cpu) {
}

uint8_t PHP(CPU *cpu) {
}

uint8_t PLA(CPU *cpu) {
}

uint8_t PLP(CPU *cpu) {
}

uint8_t ROL(CPU *cpu) {
}

uint8_t ROR(CPU *cpu) {
}

uint8_t RTI(CPU *cpu) {
}

uint8_t RTS(CPU *cpu) {
}

uint8_t SBC(CPU *cpu) {
 /*
 * A = A - M - (1 - C) (subtract with carry)
 * If C=0, borrow 1 from A, if C=1, no borrow
 *  -M = ~M + 1
 *  A = A + ~M + C
 *  set C if temp | 0xFF00 because 8th bit will be 1 if result is positive
 */
	fetch(cpu);
	uint16_t value = cpu->fetched ^ 0x00FF;
	cpu->temp = cpu->a + value + CPU_get_flag(cpu,FLAG_CARRY);
	CPU_set_flag(cpu,FLAG_CARRY,cpu->temp & 0xFF00);
	CPU_set_flag(cpu, FLAG_ZERO,(cpu->temp & 0x00FF) == 0);
	CPU_set_flag(cpu, FLAG_OVERFLOW,(cpu->temp ^ cpu->a) & (cpu->temp ^ value) & 0x0080);
	CPU_set_flag(cpu, FLAG_NEGATIVE,cpu->temp & 0x80);
	cpu->a = cpu->temp & 0x00FF;
	return 1;
}

uint8_t SEC(CPU *cpu) {
}

uint8_t SED(CPU *cpu) {
}

uint8_t SEI(CPU *cpu) {
}

uint8_t STA(CPU *cpu) {
}

uint8_t STX(CPU *cpu) {
}

uint8_t STY(CPU *cpu) {
}

uint8_t TAX(CPU *cpu) {
}

uint8_t TAY(CPU *cpu) {
}

uint8_t TSX(CPU *cpu) {
}

uint8_t TXA(CPU *cpu) {
}
/**/
uint8_t TXS(CPU *cpu) {
}

uint8_t TYA(CPU *cpu) {
}

uint8_t XXX() {
}

uint8_t IMP(CPU *cpu) {
}

uint8_t IMM(CPU *cpu) {
	/*next byte as read value*/
	cpu->addr_abs = cpu->pc++;
	return 0;
}

uint8_t ZP0(CPU *cpu) {
}

uint8_t ZPX(CPU *cpu) {
}

uint8_t ZPY(CPU *cpu) {
}

uint8_t REL(CPU *cpu) {
}

uint8_t ABS(CPU *cpu) {
}

uint8_t ABX(CPU *cpu) {
}

uint8_t ABY(CPU *cpu) {
}

uint8_t IND(CPU *cpu) {
}

uint8_t IZX(CPU *cpu) {
}

uint8_t IZY(CPU *cpu) {
}


