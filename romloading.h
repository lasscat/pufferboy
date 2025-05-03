#ifndef ROMLOADING_H     // If ROMLOADING_H is not defined, enter this block
#define ROMLOADING_H     
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    uint8_t entry[4]; //array of 4 bits for the intrsuction idk
    uint8_t nintylogo[0x30]; 
    uint8_t cgbflag; //014Aonly, 1 = hybrid , 2 no cgb
    uint16_t nlc;
    bool sgbflag; //false no sgb flag, true sgb enhacned features
    uint8_t carttype;
    uint8_t romsize;
    uint8_t ramsize;
    uint8_t destcode;
    uint8_t olc;
    uint8_t romvernum;
    uint8_t headerchecksum;
    uint16_t globalchecksum;
    uint8_t manucode[4];
    char title[16];


} romheaderinfo;
extern char useBootRom;


char* getrom(void);

void loadrom(char* romname);

void getrealromName(romheaderinfo* rhi);

void getmanucode(romheaderinfo* rhi);

void getcgbflag(romheaderinfo* rhi);

void getSGBFLAG(romheaderinfo* rhi);

void getcarttype(romheaderinfo* rhi);

void getheaderromsize(romheaderinfo* rhi);

void getramsize(romheaderinfo* rhi);

void getdestcode(romheaderinfo* rhi);

void getromvernumber(romheaderinfo* rhi);

void getheaderchecksum(romheaderinfo* rhi);

void getglobalchecksum(romheaderinfo* rhi);

void loadbootrom();

#endif