#ifndef CPU_H     // If CPU_H is not defined, enter this block
#define CPU_H     // Define CPU_H to prevent re-inclusion

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define ZERO_FLAG (uint8_t)128
#define SUBTRACTION_FLAG (uint8_t)64
#define HALF_CARRY_FLAG (uint8_t)32
#define CARRY_FLAG (uint8_t)16


typedef struct{
    //8 bit registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint8_t f;

    //16 bit registers
    uint16_t pc;
    uint16_t sp;

    uint64_t tcycles;

    //cycle
} Registers;

typedef enum{
    ZEROFLAG,
    NSUBTRACTION,
    HALFCARRYFLAG,
    CARRYFLAG,
} flagstates;

void cpuInit();


// code to get register state
bool returnFlagStateZero(Registers* reg);
bool returnFlagStateSubtraction(Registers* reg);
bool returnFlagStateHalfCarry(Registers* reg);
bool returnFlagStateCarry(Registers* reg);

//uint16_t setAF(Registers* reg, uint8_t a_value, uint8_t f_value); // code to combine the registers
void setAF(Registers* reg, uint8_t value_a, uint8_t value_f);
uint16_t returnAF(Registers* reg);

void setBC(Registers* reg, uint8_t value_b, uint8_t value_c);
uint16_t returnBC(Registers* reg);

void setDE(Registers* reg, uint8_t value_d, uint8_t value_e);
uint16_t returnDE(Registers* reg);

void setHL(Registers* reg, uint8_t value_h, uint8_t value_l);
uint16_t returnHL(Registers* reg);
//probably add something to write to it?? to the flag states

void initCPU(Registers *reg);

uint8_t fetch(Registers* reg);

void cpuStep(Registers *reg);

void debugPrintRegisters(Registers *reg);

void splitReg(uint8_t *lowbyte, uint8_t *highbyte, uint16_t value);

void setFlagStateZero(Registers* reg);
void clearFlagStateZero(Registers* reg);

void setFlagStateSubtraction(Registers* reg);
void clearFlagStateSubtraction(Registers* reg);


void setFlagStateHalfCarry(Registers* reg);
void clearFlagStateHalfCarry(Registers* reg);

void setFlagStateCarry(Registers* reg);
void clearFlagStateCarry(Registers* reg);

void PUSH(Registers* reg, uint16_t value);
void PUSH8(Registers* reg, uint8_t lowbyte, uint8_t highbyte);
uint16_t POP(Registers* reg);
uint16_t Byteto2(uint8_t lowbyte, uint8_t highbyte);
#endif  // End of CPU_H check