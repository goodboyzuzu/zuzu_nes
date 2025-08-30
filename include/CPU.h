#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef CPU_H
#define CPU_H

typedef struct {
    uint8_t a;     // Accumulator
    uint8_t x;     // X Register
    uint8_t y;     // Y Register
    uint8_t sp;    // Stack Pointer
    uint16_t pc;   // Program Counter
    uint8_t status; // Status Register
} CPU;

void CPU_init(CPU* cpu);
void CPU_destroy(CPU* cpu);

void CPU_init(CPU* cpu){
    printf("CPU_init called\n");
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->sp = 0x00;
    cpu->pc = 0x0000;
    cpu->status = 0x00;
}
void CPU_destroy(CPU* cpu){
    printf("CPU_destroy called\n");
}

#endif //CPU_H