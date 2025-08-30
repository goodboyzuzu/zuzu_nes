#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "CPU.h"

#ifndef BUS_H
#define BUS_H
typedef struct {
    CPU cpu;
    // 64KB RAM
    uint8_t ram[64*1024];
} Bus;

void Bus_init(Bus* bus);
void Bus_destroy(Bus* bus);
void Bus_write(Bus* bus, uint16_t addr, uint8_t data);
uint8_t Bus_read(Bus* bus, uint16_t addr, bool readOnly);

void Bus_init(Bus* bus){
    printf("Bus_init called\n");
}

void Bus_destroy(Bus* bus){
    printf("Bus_destroy called\n");
}



#endif // BUS_H