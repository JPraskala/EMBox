#include "vdp1.h"
// static volatile MemoryMap mMap = {0x000000, 0x080000, 0x0C0000, 0x100000, 0x180000};
// static volatile SystemRegisters systemRegisters = {0x100000, 0x100002, 0x100004, 0x100008, 0x10000A, 0x10000C, 0x100000, 0x100006, 0x100010, 0x100012, 0x100014, 0x100016};
// static volatile AdditionalRegisters additionalRegisters = {0x1E, 0x20, 0x8, 0x0};

VDP1* initVDP1() {
    /*
    systemRegisters.TVMR = 0x0;
    systemRegisters.PTMR = 0x2;
    systemRegisters.FBCR = 0x0;
    systemRegisters.EWDR = 0x0;

    systemRegisters.EWLR = 0x0;
    systemRegisters.EWRR = ((0x140 >> 0x3) << 0x9) | 0xE0;
     */

}

void drawSprite(SpriteMode mode, MemoryMap* mMap) {

}

static MemoryMap* initMemoryMap() {

}

static SystemRegisters* createRegisters() {

}

static AdditionalRegisters* createAdditionalRegisters() {

}