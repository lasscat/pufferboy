#ifndef BUS_H     // If BUS_H is not defined, enter this block
#define BUS_H     // Define BUS_H to prevent multiple inclusions
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t busRead(uint16_t address);

void busWrite(uint16_t address, uint8_t value);

#endif