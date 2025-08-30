#include "Bus.h"
#include <stdlib.h>

/* forward declaration of CPU related function */
CPU* CPU_init(Bus* bus);
void CPU_connect_Bus(CPU* cpu, Bus* bus);
void CPU_destroy(CPU** cpu);

Bus* Bus_init() {
    Bus* bus = malloc(sizeof(Bus));
    printf("Bus initialized\n");
    bus->cpu = CPU_init(bus);
    CPU_connect_Bus(bus->cpu, bus);
    return bus;
}

void Bus_destroy(Bus** bus){
    if (bus && *bus) {
        free(*bus);
        *bus = NULL;
        printf("Bus destroyed\n");
    }
    CPU_destroy(&((*bus)->cpu));
}

void Bus_write(Bus* bus, uint16_t addr, uint8_t data){
    if (addr >=0x0000 && addr <= 0xFFFF){
        bus->ram[addr] = data;
    }
}

uint8_t Bus_read(Bus* bus, uint16_t addr, bool readOnly){
    if (addr >= 0x0000 && addr <= 0xFFFF){
        return bus->ram[addr];
    }
}
