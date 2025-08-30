#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef CPU_H
#define CPU_H

typedef struct {
    uint8_t a;     /* Accumulator */
    uint8_t x;     /* X Register */
    uint8_t y;     /* Y Register */
    uint8_t sp;    /* Stack Pointer */
    uint16_t pc;   /* Program Counter */
    uint8_t status; /* Status Register */
} CPU;

void CPU_init(CPU* cpu);
void CPU_destroy(CPU* cpu);


#endif /* CPU_H */