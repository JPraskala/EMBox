#ifndef SH2_CORE
#define SH2_CORE

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int LongWord;

typedef struct {
	LongWord R[16];
	LongWord SR;
	LongWord GBR;
	LongWord VBR;
	LongWord MACH;
	LongWord MACL;
	LongWord PC;
} SH2;

Byte Read_Byte(LongWord addr);
Word Read_Word(LongWord addr);
LongWord Read_LongWord(LongWord addr);

Byte Write_Byte(LongWord addr, LongWord data);
Word Write_Word(LongWord addr, LongWord data);
LongWord Write_LongWord(LongWord addr, LongWord data);

#endif
