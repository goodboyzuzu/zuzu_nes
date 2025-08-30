#include "Bus.h"
#include "CPU.h"
#include <stdio.h>

int main()
{
    Bus* bus = Bus_init();
    Bus_destroy(&bus);
    return 0;
}