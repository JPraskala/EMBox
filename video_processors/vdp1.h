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


typedef enum {NORMAL, SCALED, DISTORTED} SpriteMode;

/*
typedef struct {
    GLubyte* sprite;
    SpriteMode mode;
    GLfloat x;
    GLfloat y;
} Coordinates;
 */

typedef enum {NON_INTERLACE, SINGLE_INTERLACE, DOUBLE_INTERLACE} ScreenMode;


void initVDP1();

GLuint VAO, VBO, EBO;

#endif