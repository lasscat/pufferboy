#ifndef MEMORY_H     // Check if MEMORY_H is not defined
#define MEMORY_H     // Define MEMORY_H to prevent redefinition

#include <stdint.h>  // Include standard integer types for uint8_t

extern uint8_t memory[0x10000];  // Declaration of the memory array
void memoryInit(void);  // Function prototype for memory initialization

#endif // End of the MEMORY_H check
