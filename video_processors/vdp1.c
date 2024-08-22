#include "vdp1.h"
static Coordinates coordinates;
static volatile MemoryMap mMap = {0x000000, 0x080000, 0x0C0000, 0x100000, 0x180000};
static volatile SystemRegisters systemRegisters = {0x100000, 0x100002, 0x100004, 0x100008, 0x10000A, 0x10000C, 0x100000, 0x100006, 0x100010, 0x100012, 0x100014, 0x100016};
static volatile AdditionalRegisters additionalRegisters = {0x1E, 0x20, 0x8, 0x0};

void openglValidation() {

}

static void enableCuda() {

}

static void printSpriteInfo(GLubyte* sprite) {

}

Byte* getGraphicsROM() {
    return NULL;
}

void displayGraphicsInfo() {

}