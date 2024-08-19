#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    Word TVHR;
    Word FBCR;
    Word PTMR;
    Word EWLR;
    Word EWRR;
    Word ENDR;
    Word TVMR;
    Word EWDR;
    const Word EDSR;
    const Word LOPR;
    const Word COPR;
    const Word MODR;
} SystemRegisters;

typedef struct {
    LongWord COMMANDTABLE;
    LongWord COLORTABLE;
    LongWord GOURAUD;
    LongWord CHARACTERPATTERN;
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
    Byte* sprite;
    SpriteMode mode;
    GLfloat x;
    GLfloat y;
} Coordinates;

typedef enum {NON_INTERLACE, SINGLE_INTERLACE, DOUBLE_INTERLACE} ScreenMode;


void openglValidation();
static void* initMemory(size_t size);
static void printSpriteInfo(Byte* sprite);
Byte* getGraphicsROM();
