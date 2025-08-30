#include "Bus.h"
#include <stdlib.h>

Bus* Bus_init() {
    Bus* bus = malloc(sizeof(Bus));
    if (bus) {
        /* Initialize fields if needed */
        printf("Bus_init called\n");
        /* print size of bus */
        printf("Size of Bus: %zu bytes\n", sizeof(Bus));
        /* memory address of bus pointer */
        printf("Bus pointer address: %p\n", (void*)bus);
    }
    return bus;
}

void Bus_destroy(Bus** bus){
    if (bus && *bus) {
        printf("Bus_destroy called\n");
        free(*bus);
        printf("Bus pointer after free: %p\n", (void*)(*bus));
        *bus = NULL;
        printf("Bus pointer after setting to NULL: %p\n", (void*)(*bus));
    }
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
