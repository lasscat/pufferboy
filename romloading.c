#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "romloading.h"
#include "memory.h"

char useBootRom = 'n';

char* getrom(void) {

    printf("are you going to use a bootrom named bootroom.bin? y/n");
    scanf(" %c", &useBootRom);
    if (useBootRom != 'y' && useBootRom != 'n'){
        printf("Not a value input!");
    }

    char* romname = malloc(100);
    if (!romname) {
        perror("malloc failed");
        return NULL;
    }

    printf("What is the name of the file for your ROM?\n");
    scanf("%99s", romname);  // prevent buffer overflow

    int romsize = strlen(romname);
    char* newromname = realloc(romname, romsize + 1);  // just enough space

    if (!newromname) {
        perror("realloc failed");
        free(romname);
        return NULL;
    }

    romname = newromname;
    printf("ROM file: %s\n", romname);

    return romname;
}

void loadrom(char* romname){ // taking out rom loading rn for bios
    //FILE *fptr;
    //int filesize;  
    /*fptr = fopen(romname, "rb");
    if (fptr == NULL){
        printf("failed to load rom\n");
    } else{
        printf("it worked yay!\n");
    }
    fseek(fptr, 0, SEEK_END);
    // printf("%ld", ftell(fptr));
    filesize = ftell(fptr);
    rewind(fptr);
    printf("%d", filesize); */

    loadbootrom();

    /*
    if (filesize >= 16384){ //okay we make sure that filesize is bigger than 16kb bc we will put the first 16 into ram
        // this is confusing for rn: printf("\n32kb");
        fread(memory, 1, 16384, fptr); // put 16kb rom into memory
        if (memory[327] == 0 && memory[328] == 0){ //check mbc if it exists and rom size 
            fread(memory + 16384, 1, 16384, fptr);
        } 
        fclose(fptr);
    } else{
        printf("is ur rom bigger than 16knb");
        fclose(fptr);
    } */

}

void getrealromName(romheaderinfo* rhi){
    int ti = 0;
    for (int i = 308; i <= 323; i++){
        rhi->title[ti] = memory[i];
        ti++;
    }
}

void getmanucode(romheaderinfo* rhi){ //unsure if this is nessacary
    int mi = 0;
    for (int i = 319; i <= 322; i++){
        rhi->manucode[mi] = memory[i];
        mi++;
    }
}

void getcgbflag(romheaderinfo* rhi){ // weird issue where cgb flag retunrs 182 or smth when false 
    //TODO ADD SUPPORT FOR DUAL CGB AND DMG GAMES
    uint8_t cgb_byte = memory[0x143];
    if (cgb_byte == 128){
        rhi->cgbflag = 1;
    } else if(cgb_byte ==  192){
        rhi->cgbflag = 0;
    } 
} // do more investgating and look at the rom in a hex viewewr and find the bitmask fro it
//get final load and check if theres something that we dont support like th ecgb flag but overide if its 32kb

void getLC(romheaderinfo* rhi){ // prob add an array to get the company but it isnt useful rn
    uint8_t olcbyte = memory[331];
    if (olcbyte !=51){
        rhi->olc = olcbyte;
    } else{
        rhi->nlc = ((uint16_t)memory[0x144] << 8) | memory[0x145];
        }
    
}

void getSGBFLAG(romheaderinfo* rhi){
    uint8_t sgbbyte = memory[326];
    if (sgbbyte == 0){
        rhi->sgbflag = false;
    } else if(sgbbyte == 3){
        rhi->sgbflag = true;
    }
}

void getcarttype(romheaderinfo* rhi){
    rhi->carttype =  memory[327];

    /*currently supported is no mbc so we will only accept 0
    if (rhi->carttype == 0){
        printf("\nnombc");
    } else{ // add for more and add a list for all the types
        printf("mbc");
    } */
}

void getheaderromsize(romheaderinfo* rhi){
    rhi->romsize = memory[328];
}

void getramsize(romheaderinfo* rhi){
    rhi->ramsize = memory[329];
}

void getdestcode(romheaderinfo* rhi){
     rhi->destcode = memory[330];
}

void getromvernumber(romheaderinfo* rhi){
    rhi->romvernum = memory[332];
}   

void getheaderchecksum(romheaderinfo* rhi){
    rhi->headerchecksum = memory[333];
}

void getglobalchecksum(romheaderinfo* rhi){
    rhi->globalchecksum = ((uint16_t)memory[334] << 8) | memory[335];
}

void loadbootrom(){ //add more checks in the future 
    //see if bootroom needs to be ran first if so then do the bootrom load function
    if (useBootRom == 'y'){ //TODO ADD CHECKS FOR FILE HASH AND SIZE IF THE USER IS AN IDIOT
        printf("\n called function boot rom \n");
        FILE *brf;
        brf = fopen("bootrom.bin", "rb"); 
        fread(memory, 1, 256, brf); // put 16kb rom into memory
        fclose(brf);
    }

}