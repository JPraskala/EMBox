#ifndef VDP1_H
#define VDP1_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define MAXSIZE 1024


typedef uint32_t LongWord;
typedef uint16_t Word;
typedef int16_t SWord;
typedef uint8_t Byte;

typedef struct {
    LongWord VRAM[MAXSIZE];
    LongWord FRAMEBUFFER[MAXSIZE];
    LongWord RESERVED[MAXSIZE];
    LongWord SYSTEMREGISTERS[MAXSIZE];
    LongWord ACCESSPROHIBITED[MAXSIZE];
} MemoryMap;

typedef struct {
    LongWord TVHR;
    LongWord FBCR;
    LongWord PTMR;
    LongWord EWLR;
    LongWord EWRR;
    LongWord ENDR;
    LongWord TVMR;
    LongWord EWDR;
    LongWord EDSR;
    LongWord LOPR;
    LongWord COPR;
    LongWord MODR;
} SystemRegisters;

typedef struct {
    Word COMMANDTABLE;
    Word COLORTABLE;
    Word GOURAUD;
    Word CHARACTERPATTERN;
} AdditionalRegisters;

typedef struct {
    Word CMDCTRL;
    Word CMDLINK;
    Word CMDPMOD;
    Word CMDCOLR;
    Word CMDSRCA;
    Word CMDSIZE;
    SWord CMDXA;
    SWord CMDYA;
    SWord CMDXB;
    SWord CMDYB;
    SWord CMDXC;
    SWord CMDYC;
    SWord CMDXD;
    SWord CMDYD;
    Word CMDGRDA;
} CommandTable;


typedef struct {
    MemoryMap* mMap;
    SystemRegisters* systemRegisters;
    AdditionalRegisters* additionalRegisters;
} VDP1;


typedef enum {NORMAL, SCALED, DISTORTED, POLYGON, POLYLINE, LINE} SpriteMode;
typedef enum {NON_INTERLACE, SINGLE_INTERLACE, DOUBLE_INTERLACE} ScreenMode;
typedef enum {MODE0, MODE1, MODE2, MODE3, MODE4, MODE5} ColorModes;
typedef enum {REPLACE, SHADOW, HALF_LUMINANCE, HALF_TRANSPARENCY, GOURAUD_SHADING} ColorCalculations; // Note Gouraud can use both half luminance and half transparency, this needs to be accounted for when working with colors.

VDP1* initVDP1();
//static void initMemory(VDP1* vdp1);
// static void initSystemRegisters(SystemRegisters* systemRegisters);
static void* initMemory(size_t size);
void drawSprite(SpriteMode mode, ColorModes colorMode, ColorCalculations colorCalculations, VDP1* vdp1);
void systemClipping(VDP1* vdp1);
void userClipping(VDP1* vdp1);
void freeVDP1(VDP1* vdp1);

#endif