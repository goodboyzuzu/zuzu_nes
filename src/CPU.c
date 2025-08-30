#include "CPU.h"

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