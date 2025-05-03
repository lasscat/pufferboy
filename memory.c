#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"

uint8_t memory[0x10000];


void memoryInit(void){
    memset(memory, 0, sizeof(memory));
}

//oid load_rom()
    
/*
void get_rom(romst* romm){
    printf("Enter the rom you want to load\n");
    scanf("%d", romm->rom_name);
}

bool load_rom(char filename); // user input to load rom then have the value that they said be the filename

*/ 