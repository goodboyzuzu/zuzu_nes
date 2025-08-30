#include "CPU.h"
#include <stdio.h>

int main()
{
    Bus* bus = Bus_init();
    CPU_write(bus->cpu, 0x00FF, 0x62);
    uint8_t value = CPU_read(bus->cpu, 0x00FF);
    printf("Value at 0x00FF: 0x%02X\n", value);
    CPU_destroy(&bus->cpu);
    Bus_destroy(&bus);
    return 0;
}