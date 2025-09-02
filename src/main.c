#include "CPU.h"
#include <stdio.h>

int main()
{
    Bus* bus = Bus_init();
    CPU* cpu = bus->cpu;
    CPU_write(cpu, 0x0000, 0x00);
    CPU_write(cpu, 0x0001, 0x01);
    CPU_write(cpu, 0x0002, 0x02);

    Bus_destroy(&bus);
    return 0;
}