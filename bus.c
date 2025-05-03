#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "memory.h"



uint8_t busRead(uint16_t address){ //add support for io and stuff later
    return memory[address];
}

void busWrite(uint16_t address, uint8_t value){
    memory[address] = value;
}