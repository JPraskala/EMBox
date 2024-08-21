#ifndef VDP1_H
#define VDP1_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <time.h>


typedef uint32_t LongWord;
typedef uint16_t Word;
typedef uint8_t Byte;

typedef struct {
    LongWord VRAM;
    LongWord FRAMEBUFFER;
    LongWord RESERVED;
    LongWord SYSTEMREGISTERS;
    LongWord ACCESSPROHIBITED;
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

/*
// Memory Map

#define VRAMSTART *(volatile u32*)0x000000
#define VRAMEND *(volatile u32*)0x07FFFFH
#define FRAMESTART *(volatile u32*)0x080000
#define FRAMEEND *(volatile u32*)0x0BFFFFH
#define RESERVEDSTART *(volatile u32*)0x0C0000
#define RESERVEDEND *(volatile u32*)0x0FFFFFH
#define SYSTEMREGISTERSSTART *(volatile u32*)0x100000
#define SYSTEMREGISTERSEND *(volatile u32*)0x17FFFFH
#define ACCESSPROHIBITEDSTART *(volatile u32*)0x180000
#define ACCESSPROHIBITEDEND *(volatile u32*)0x1FFFFH

// System Registers

// WRITE ONLY!!!
#define TVHR *(volatile u32*)0x100000H
#define FBCR *(volatile u16*)0x100002H
#define PTMR *(volatile u16*)0x100004H
#define EWLR *(volatile u16*)0x100008H
#define EWRR *(volatile u16*)0x10000AH
#define ENDR *(volatile u16*)0x10000CH
#define TVMR *(volatile u16*)0x100000H
#define EWDR *(volatile u16*)0x100006H


// READ ONLY!!!

#define EDSR *(volatile u16*)0x100010H
#define LOPR *(volatile u16*)0x100012H
#define COPR *(volatile u16*)0x100014H
#define MODR *(volatile u16*)0x100016H


// Additional Registers

#define COMMANDTABLESIZE *(volatile u32*)0x1EH
#define COMMANDTABLEBOUNDARY *(volatile u32*)0x20H
#define COLORTABLESIZE *(volatile u32*)0x20H
#define COLORTABLEBOUNDARY *(volatile u32*)0x20H
#define GOURAUDSIZE *(volatile u32*)0x8H
#define GOURAUDBOUNDARY *(volatile u32*)0x8H
#define CHARACTERPATTERNSIZE *(volatile u32*)0x00
#define CHARACTERPATTERNBOUNDARY *(volatile u32*)0x20H
*/


typedef enum {NORMAL, SCALED, DISTORTED} SpriteMode;

typedef struct {
    GLubyte* sprite;
    SpriteMode mode;
    GLfloat x;
    GLfloat y;
} Coordinates;

typedef enum {NON_INTERLACE, SINGLE_INTERLACE, DOUBLE_INTERLACE} ScreenMode;


bool openglValidation();
static bool cudaTest();
static volatile MemoryMap* createMemoryMap();
static volatile SystemRegisters* createSystemRegisters();
static volatile AdditionalRegisters* createAdditionalRegisters();
static void printSpriteInfo(GLubyte* sprite);
Byte* getGraphicsROM();

#endif