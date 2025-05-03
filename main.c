#include <stdio.h>
#include "cpu.h"
#include "romloading.h"
#include "memory.h"
#include "bus.h"

/*
TODO
bus support bus read and joypad get state eventually
mbc support drawing
and instructions
see grok for more info

add support for roms that are bigger than 32kb

initrom load load first 16kb
then final load and get settings and see if it is supported by the emulator
include the .h files instead of .c
*/
int main(){
    char* rom;
    Registers reg;
    //romheaderinfo rhi;
    initCPU(&reg);
    memoryInit();
    rom = getrom();
    loadrom(rom);
    cpuStep(&reg);
    debugPrintRegisters(&reg);
}
