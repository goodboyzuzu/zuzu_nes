#include "Bus.h"

int main()
{
    Bus bus;
    CPU cpu;
    Bus_init(&bus);
    Bus_destroy(&bus);
    CPU_init(&cpu);
    CPU_destroy(&cpu);
}