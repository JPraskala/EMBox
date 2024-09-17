#include "vdp1.h"
// static volatile MemoryMap mMap = {0x000000, 0x080000, 0x0C0000, 0x100000, 0x180000};
// static volatile SystemRegisters systemRegisters = {0x100000, 0x100002, 0x100004, 0x100008, 0x10000A, 0x10000C, 0x100000, 0x100006, 0x100010, 0x100012, 0x100014, 0x100016};
// static volatile AdditionalRegisters additionalRegisters = {0x1E, 0x20, 0x8, 0x0};

VDP1* initVDP1() {
    VDP1* vdp1 = (VDP1*) malloc(sizeof(VDP1));
    if (vdp1 == NULL) {
        printf("Failed to create the vdp1 struct.");
        return NULL;
    }

    vdp1->mMap = (MemoryMap*) initMemory(sizeof(MemoryMap));
    vdp1->systemRegisters = (SystemRegisters*) initMemory(sizeof(SystemRegisters));
    vdp1->additionalRegisters = (AdditionalRegisters*) initMemory(sizeof(AdditionalRegisters));
    return vdp1->mMap == NULL || vdp1->systemRegisters == NULL || vdp1->additionalRegisters == NULL ? NULL : vdp1;
}


static void* initMemory(size_t size) {
    return malloc(size);
}

/*
static void initMemory(VDP1* vdp1) {
    if (memoryInitialized) {
        return;
    }

    vdp1->mMap = (MemoryMap*) malloc(sizeof(MemoryMap));
    vdp1->systemRegisters = (SystemRegisters*) malloc(sizeof(SystemRegisters));
    vdp1->additionalRegisters = (AdditionalRegisters*) malloc(sizeof(AdditionalRegisters));
    if (vdp1->mMap == NULL || vdp1->systemRegisters == NULL || vdp1->additionalRegisters == NULL) {
        printf("Failed to initialize the memory map and registers within VDP1.");
        exit(EXIT_FAILURE);
    }

    // initSystemRegisters(vdp1->systemRegisters);

    // Memory Map Initialization
    vdp1->mMap->VRAM = (LongWord*) malloc(MAXSIZE * sizeof(LongWord));
    vdp1->mMap->FRAMEBUFFER = (LongWord*) malloc(MAXSIZE * sizeof(LongWord));
    vdp1->mMap->ACCESSPROHIBITED = (LongWord*) malloc(MAXSIZE * sizeof(LongWord));
    vdp1->mMap->RESERVED = (LongWord*) malloc(MAXSIZE * sizeof(LongWord));
    vdp1->mMap->SYSTEMREGISTERS = (LongWord*) malloc(MAXSIZE * sizeof(LongWord));
    if (vdp1->mMap->SYSTEMREGISTERS == NULL || vdp1->mMap->RESERVED == NULL || vdp1->mMap->ACCESSPROHIBITED == NULL || vdp1->mMap->FRAMEBUFFER == NULL || vdp1->mMap->VRAM == NULL) {
        printf("Failed to initialize the memory map for the vdp1.");
        exit(EXIT_FAILURE);
    }

    // Additional Registers Initialization
    vdp1->additionalRegisters->COMMANDTABLE = 0x1E;
    vdp1->additionalRegisters->COLORTABLE = 0x20;
    vdp1->additionalRegisters->GOURAUD = 0x8;
    vdp1->additionalRegisters->CHARACTERPATTERN = 0x0;

    // System Registers Initialization
    vdp1->systemRegisters->TVHR = 0x100000;
    vdp1->systemRegisters->FBCR = 0x0;
    vdp1->systemRegisters->PTMR = 0x2;
    vdp1->systemRegisters->EWLR = 0x0;
    vdp1->systemRegisters->EWRR = ((0x140 >> 0x3) << 0x9) | 0xE0;
    vdp1->systemRegisters->ENDR = 0x10000C;
    vdp1->systemRegisters->TVMR = 0x0;
    vdp1->systemRegisters->EWDR = 0x0;
    vdp1->systemRegisters->EDSR = 0x100010;
    vdp1->systemRegisters->LOPR = 0x100012;
    vdp1->systemRegisters->COPR = 0x100014;
    vdp1->systemRegisters->MODR = 0x100016;
    memoryInitialized = true;
}
 */

void freeVDP1(VDP1* vdp1) {
    /*
    free(vdp1->mMap->FRAMEBUFFER);
    free(vdp1->mMap->ACCESSPROHIBITED);
    free(vdp1->mMap->RESERVED);
    free(vdp1->mMap->SYSTEMREGISTERS);
    free(vdp1->mMap->VRAM);
    */

    free(vdp1->mMap);
    free(vdp1->systemRegisters);
    free(vdp1->additionalRegisters);

    free(vdp1);
}
