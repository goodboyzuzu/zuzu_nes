#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "Bus.h"

#ifndef CPU_H
#define CPU_H

/* forward declaration for CPU struct. As instruction struct uses CPU pointer () */
typedef struct INSTRUCTION INSTRUCTION;

typedef struct CPU{
    uint8_t a;     /* Accumulator */
    uint8_t x;     /* X Register */
    uint8_t y;     /* Y Register */
    uint8_t sp;    /* Stack Pointer */
    uint16_t pc;   /* Program Counter */
    uint8_t status; /* Status Register */

    INSTRUCTION* lookup_table;
    Bus* bus;

	uint8_t  fetched;     /* Represents the working input value to the ALU*/
	uint16_t temp;        /* A convenience variable used everywhere*/
	uint16_t addr_abs;    /* All used memory addresses end up in here*/
	uint16_t addr_rel;    /* Represents absolute address following a branch*/
	uint8_t  opcode;      /* Is the instruction byte*/
	uint8_t  cycles;      /* Counts how many cycles the instruction has remaining*/
	uint32_t clock_count; /* A global accumulation of the number of clocks*/
} CPU;


typedef struct INSTRUCTION{
    const char * name;
    uint8_t (*operate)(CPU* cpu);
    uint8_t (*addrmode)(CPU* cpu);
    uint8_t cycles;
} INSTRUCTION;

extern INSTRUCTION lookup[];

CPU* CPU_init();
void CPU_destroy(CPU** cpu);
void CPU_connect_Bus(CPU* cpu, Bus* bus);
void CPU_write(CPU* cpu, uint16_t a, uint8_t d);
uint8_t CPU_read(CPU* cpu, uint16_t a);

uint8_t fetch(CPU* cpu); /* decide if data comes from memory address or instruction */

/* external event function */
void CPU_reset(CPU* cpu); /* Reset interrupt- force CPU into known state */
void CPU_irq(CPU* cpu); /* interrupt request- execute instruction at a specific location */
void CPU_nmi(CPU* cpu); /* non-maskable interrupt request: same as irq but cannot be disabled */
void CPU_clock(CPU* cpu); /* perform 1 clock cycle */

bool CPU_complete(CPU* cpu); /* indicate instruction complete, no need to clock every cycle */

/* to do */
int CPU_disassemble(CPU* cpu, uint16_t start, uint16_t end);

enum CPU_FLAGS
{
    c = (1 << 0),	/*carry bit*/
    z = (1 << 1),	/*zero*/
    i = (1 << 2),	/*disable interrupts*/
    d = (1 << 3),	/*decimal mode (unused in this implementation)*/
    b = (1 << 4),	/*break*/
    u = (1 << 5),	/*unused*/
    v = (1 << 6),	/*overflow*/
    n = (1 << 7),	/*negative*/
};

uint8_t GetFlag(CPU* cpu, enum CPU_FLAGS f);
void SetFlag(CPU* cpu, enum CPU_FLAGS f, bool v);

/* opcodes */
uint8_t ADC(CPU* cpu);	/* uint8_t AND(CPU* cpu);	uint8_t ASL(CPU* cpu);	uint8_t BCC(CPU* cpu);
uint8_t BCS(CPU* cpu);	uint8_t BEQ(CPU* cpu);	uint8_t BIT(CPU* cpu);	uint8_t BMI(CPU* cpu);
uint8_t BNE(CPU* cpu);	uint8_t BPL(CPU* cpu);	uint8_t BRK(CPU* cpu);	uint8_t BVC(CPU* cpu);
uint8_t BVS(CPU* cpu);	uint8_t CLC(CPU* cpu);	uint8_t CLD(CPU* cpu);	uint8_t CLI(CPU* cpu);
uint8_t CLV(CPU* cpu);	uint8_t CMP(CPU* cpu);	uint8_t CPX(CPU* cpu);	uint8_t CPY(CPU* cpu);
uint8_t DEC(CPU* cpu);	uint8_t DEX(CPU* cpu);	uint8_t DEY(CPU* cpu);	uint8_t EOR(CPU* cpu);
uint8_t INC(CPU* cpu);	uint8_t INX(CPU* cpu);	uint8_t INY(CPU* cpu);	uint8_t JMP(CPU* cpu);
uint8_t JSR(CPU* cpu);	uint8_t LDA(CPU* cpu);	uint8_t LDX(CPU* cpu);	uint8_t LDY(CPU* cpu);
uint8_t LSR(CPU* cpu);	uint8_t NOP(CPU* cpu);	uint8_t ORA(CPU* cpu);	uint8_t PHA(CPU* cpu);
uint8_t PHP(CPU* cpu);	uint8_t PLA(CPU* cpu);	uint8_t PLP(CPU* cpu);	uint8_t ROL(CPU* cpu);
uint8_t ROR(CPU* cpu);	uint8_t RTI(CPU* cpu);	uint8_t RTS(CPU* cpu);	uint8_t SBC(CPU* cpu);
uint8_t SEC(CPU* cpu);	uint8_t SED(CPU* cpu);	uint8_t SEI(CPU* cpu);	uint8_t STA(CPU* cpu);
uint8_t STX(CPU* cpu);	uint8_t STY(CPU* cpu);	uint8_t TAX(CPU* cpu);	uint8_t TAY(CPU* cpu);
uint8_t TSX(CPU* cpu);	uint8_t TXA(CPU* cpu);	uint8_t TXS(CPU* cpu);	uint8_t TYA(CPU* cpu);
 */
/* addressing */
uint8_t IMP(CPU* cpu);	/* uint8_t IMM(CPU* cpu);	
uint8_t ZP0(CPU* cpu);	uint8_t ZPX(CPU* cpu);	
uint8_t ZPY(CPU* cpu);	uint8_t REL(CPU* cpu);
uint8_t ABS(CPU* cpu);	uint8_t ABX(CPU* cpu);	
uint8_t ABY(CPU* cpu);	uint8_t IND(CPU* cpu);	
uint8_t IZX(CPU* cpu);	uint8_t IZY(CPU* cpu);
 */
#endif /* CPU_H */