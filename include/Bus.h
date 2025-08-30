#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef BUS_H
#define BUS_H

typedef struct CPU CPU;

typedef struct Bus{
    CPU* cpu;
    /* 64KB RAM */
    uint8_t ram[64*1024];
} Bus;

Bus* Bus_init();
void Bus_destroy(Bus** bus);
void Bus_write(Bus* bus, uint16_t addr, uint8_t data);
uint8_t Bus_read(Bus* bus, uint16_t addr, bool readOnly);




#endif /* BUS_H */