#ifndef VDP1_H
#define VDP1_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <time.h>


typedef uint32_t LongWord;
typedef uint16_t Word;
typedef int16_t SWord;
typedef uint8_t Byte;

typedef struct {
    LongWord* VRAM;
    LongWord* FRAMEBUFFER;
    LongWord* RESERVED;
    LongWord* SYSTEMREGISTERS;
    LongWord* ACCESSPROHIBITED;
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
    const LongWord EDSR;
    const LongWord LOPR;
    const LongWord COPR;
    const LongWord MODR;
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
static MemoryMap* initMemoryMap();
static SystemRegisters* createRegisters();
static AdditionalRegisters* createAdditionalRegisters();
void createColors(ColorModes colorMode, ColorCalculations colorCalc);
void drawSprite(SpriteMode mode, MemoryMap* mMap);
void systemClipping();
void userClipping();
void freeVDP1(VDP1* vdp1);

#endif