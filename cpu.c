#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "bus.h"
#include "memory.h"

bool returnFlagStateZero(Registers* reg){
    return reg->f & ZERO_FLAG;
}

bool returnFlagStateSubtraction(Registers* reg){
    return reg->f & SUBTRACTION_FLAG;
}

bool returnFlagStateHalfCarry(Registers* reg){
    return reg->f & HALF_CARRY_FLAG;
}

bool returnFlagStateCarry(Registers* reg){
    return reg->f & CARRY_FLAG;
}

///////////////////////////////////////////////////////

void setFlagStateZero(Registers* reg){
    reg->f = reg->f | ZERO_FLAG;
}
void clearFlagStateZero(Registers* reg){
    reg->f &= ~ZERO_FLAG;
}

void setFlagStateSubtraction(Registers* reg){
    reg->f = reg->f | SUBTRACTION_FLAG;
}
void clearFlagStateSubtraction(Registers* reg){
    reg->f &= ~SUBTRACTION_FLAG;
}

void setFlagStateHalfCarry(Registers* reg){
    reg->f = reg->f | HALF_CARRY_FLAG;
}

void clearFlagStateHalfCarry(Registers* reg){
    reg->f &= ~HALF_CARRY_FLAG;
}

void setFlagStateCarry(Registers* reg){
    reg->f = reg->f | CARRY_FLAG;
}

void clearFlagStateCarry(Registers* reg){
    reg->f &= ~CARRY_FLAG;
}


/*uint16_t setAF(Registers* reg, uint8_t a_value, uint8_t f_value){
    reg->a = a_value;
    reg->f = f_value;
    reg->f = f_value & 0xF0;
    return ((uint16_t)reg->a <<8) | reg->f;
}
*/
void setAF(Registers* reg, uint8_t value_a, uint8_t value_f){
    reg->a = value_a;
    reg->f = value_f;
}
uint16_t returnAF(Registers* reg){
    return((uint16_t)reg->a << 8) | reg->f;
}


void setBC(Registers* reg, uint8_t value_b, uint8_t value_c){
    reg->b = value_b;
    reg->c = value_c;
}
uint16_t returnBC(Registers* reg){
    return ((uint16_t)reg->b << 8) | reg->c;
}

void setDE(Registers* reg, uint8_t value_d, uint8_t value_e){
    reg->d = value_d;
    reg->e = value_e;
}

uint16_t returnDE(Registers* reg){
    return ((uint16_t)reg->d << 8) | reg->e;
}

void setHL(Registers* reg, uint8_t value_h, uint8_t value_l){
    reg->h = value_h;
    reg->l = value_l;
}
uint16_t returnHL(Registers* reg){
    return ((uint16_t) reg->h << 8) | reg->l;
}

void splitReg(uint8_t *lowbyte, uint8_t *highbyte, uint16_t value){
    *lowbyte = (value & 0xFF);
    *highbyte = (value >> 8) & 0xFF;
}

void PUSH(Registers* reg, uint16_t value){
    reg->sp -=2;
    memory[reg->sp] = ((uint8_t) value); //low byte
    memory[reg->sp + 1] = ((uint8_t)value >> 8); //high byte
}

void PUSH8(Registers* reg, uint8_t lowbyte, uint8_t highbyte){
    reg->sp -=2;
    memory[reg->sp] = lowbyte;
    memory[reg->sp + 1] = highbyte;
}

uint16_t POP(Registers* reg){
    uint8_t lowbyte = memory[reg->sp];
    uint8_t highbyte = memory[reg->sp + 1];
    reg->sp +=2;
    return ((uint16_t) highbyte << 8) | lowbyte;
}

uint16_t Byteto2(uint8_t lowbyte, uint8_t highbyte){
    return ((uint16_t) highbyte << 8) | lowbyte;
}

/*void initCPU(Registers *reg){
    reg->a = 0x01;
    reg->f = 0xB0;
    reg->b = 0x00;
    reg->c = 0x13;
    reg->d = 0x00;
    reg->e = 0xD8;
    reg->h = 0x01;
    reg->l = 0x4D;
    reg->sp = 0xFFFE;
    reg->pc = 0x0100;
} */
void initCPU(Registers *reg){
    reg->a = 0;
    reg->f = 0;
    reg->b = 0;
    reg->c = 0;
    reg->d = 0;
    reg->e = 0;
    reg->h = 0;
    reg->l = 0;
    reg->sp = 0;
    reg->pc = 0;
    reg->tcycles = 0;
}

uint8_t fetch(Registers* reg){
    uint8_t opcode = busRead(reg->pc);
    reg->pc++;
    return opcode;
}

void cpuStep(Registers *reg){
    uint8_t opcode = fetch(reg); //current instruction

    switch (opcode){// instructiosn ;p
        case 0x00:
            printf("nop");
            reg->tcycles += 4;
            break;
        case 0x01: //LD BC, d16
            uint8_t c = busRead(reg->pc+1);
            uint8_t b = busRead(reg->pc+2);
            setBC(reg, b, c);
            reg->tcycles +=12;
            reg->pc += 2;
            break;
        case 0x02: //LD (BC), A
            uint16_t bcaddress = returnBC(reg);
            busWrite(bcaddress, reg->a);
            reg->tcycles +=8;
            break;
        case 0x03: //INC BC
            uint16_t bcpair = returnBC(reg);
            bcpair +=1;
            uint8_t hb, lb;
            splitReg(&lb, &hb, bcpair);
            setBC(reg, hb, lb);
            reg->tcycles +=8;
            break;
        case 0x04: //INC B
            clearFlagStateSubtraction(reg);
            if ((reg->b & 0x0F) + (1 & 0x0F) > 0x0F){
                setFlagStateHalfCarry(reg);
            } else {
                clearFlagStateHalfCarry(reg);  // Clear the Half Carry flag if no carry from bit 4
            }
            reg->b +=1;
            if (reg->b == 0){
                setFlagStateZero(reg);
            } else{
                clearFlagStateZero(reg);
            }
            reg->tcycles +=4;
            break;
        case 0x05: //dec B
            setFlagStateSubtraction(reg);
            if ((reg->b & 0x0F) - (1 & 0x0F) < 0x00){
                setFlagStateHalfCarry(reg);
            } else{
                clearFlagStateHalfCarry(reg);
            }
            reg->b -=1;
            if (reg->b == 0){
                setFlagStateZero(reg);
            } else{
                clearFlagStateZero(reg);
            }
            reg->tcycles +=4;
            break;  
        case 0x06: //LD B, d
            uint8_t value = busRead(reg->pc+1);
            reg->b = value;
            reg->pc +=1;
            reg->tcycles +=8;
            break;
        case 0x07: //RLCA
            clearFlagStateSubtraction(reg);
            clearFlagStateHalfCarry(reg);
            clearFlagStateZero(reg);
            uint8_t carriedbit = (reg->a & 128) >> 7;
            reg->a = (reg->a << 1) | carriedbit;
            if (carriedbit == 1){
                setFlagStateCarry(reg); // flag state actived bc its carryign a one
            } else{
                clearFlagStateCarry(reg);
            }
            reg->tcycles +=4;
            break;
        case 0x08: //LD (u16),SP - 0x08
            uint8_t lowbyte = memory[reg->pc+1];
            uint8_t highbyte = memory[reg->pc+2];
            uint16_t nnn = Byteto2(lowbyte, highbyte); //change this
            memory[nnn] = reg->sp & 0xFF;
            memory[nnn + 1] = (reg->sp >> 8) & 0xFF;
            reg->pc +=2;
            reg->tcycles += 20;
            break;
        case 0x09:
            clearFlagStateSubtraction(reg);
            uint16_t bcval = returnBC(reg);
            uint16_t hlval = returnHL(reg);
            uint16_t result = bcval + hlval;
            uint8_t resultlowbyte = (uint8_t)(result & 0xFF);
            uint8_t resulthighbyte = (uint8_t)(result >> 8);
            setHL(reg, resulthighbyte, resultlowbyte);
            if((bcval & 0x0FFF) + (hlval & 0x0FFF) > 0x0FFF){
                setFlagStateHalfCarry(reg);
            } else{
                clearFlagStateHalfCarry(reg);
            }
            if((bcval + hlval) > 0xFFFF){
                setFlagStateCarry(reg);
            } else{
                clearFlagStateCarry(reg);
            }
            reg->tcycles +=8;
            break;

        case 0x31:
            uint8_t nlowbyte = memory[reg->pc+1];
            uint8_t nhighbyte = memory[reg->pc+2];
            uint16_t nn = Byteto2(nlowbyte, nhighbyte);
            reg->sp = nn;
            reg->pc += 2;
            reg->tcycles +=12;
            break;
        
        default:
            printf("CPU INSTRUCTION NOT FOUND at 0x%04X, opcode: 0x%02X\n", reg->pc - 1, opcode);
            break;
    }  // instructiosn ;p
}

void debugPrintRegisters(Registers *reg){
    printf("\nregister a: 0x%x\n", reg->a);
    printf("\nregister f: 0x%x\n", reg->f);
    printf("\nregister b: 0x%x\n", reg->b);
    printf("\nregister c: 0x%x\n", reg->c);
    printf("\nregister d: 0x%x\n", reg->d);
    printf("\nregister e: 0x%x\n", reg->e);
    printf("\nregister h: 0x%x\n", reg->h);
    printf("\nregister l: 0x%x\n", reg->l);
    printf("\nregister sp: 0x%x\n", reg->sp);
    printf("\nregister pc: 0x%x\n", reg->pc);
}
//void executeInstruction(Registers*reg, uint8_t opcode);