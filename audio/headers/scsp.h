#pragma once
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

// Individual Slot Control Registers
#define KYONEX *(volatile u32*)0x00
#define KYONB *(volatile u32*)0x00
#define SBCTL *(volatile u32*)0x00
#define SSCTL *(volatile u32*)0x00
#define LPCTL *(volatile u32*)0x00
#define PCM8B *(volatile u32*)0x00
#define SA *(volatile u32*)0x00

#define SA2 *(volatile u32*)0x02

#define LSA *(volatile u32*)0x04

#define LEA *(volatile u32*)0x06

#define D2R *(volatile u32*)0x08
#define D1R *(volatile u32*)0x08
#define EGHOLD *(volatile u32*)0x08
#define AR *(volatile u32*)0x08

#define LPSLNK *(volatile u32*)0x0A
#define KRS *(volatile u32*)0x0A
#define DL *(volatile u32*)0x0A
#define RR *(volatile u32*)0x0A

#define STWINH *(volatile u32*)0x0C
#define SDIR *(volatile u32*)0x0C
#define TL *(volatile u32*)0x0C

#define MDL *(volatile u32*)0x0E
#define MDXSL *(volatile u32*)0x0E
#define MDYSL *(volatile u32*)0x0E

#define OCT *(volatile u32*)0x10
#define FNS *(volatile u32*)0x10

#define LFORE *(volatile u32*)0x12
#define LFOF *(volatile u32*)0x12
#define PLFOWS *(volatile u32*)0x12
#define PLFOS *(volatile u32*)0x12
#define ALFOWS *(volatile u32*)0x12
#define ALFOS *(volatile u32*)0x12

#define ISEL *(volatile u32*)0x14
#define OMXL *(volatile u32*)0x14

#define DISDL *(volatile u32*)0x16
#define DIPAN *(volatile u32*)0x16
#define EFSDL *(volatile u32*)0x16
#define EFPAN *(volatile u32*)0x16

// Control Registers

#define MEM4MB *(volatile u32*)0x25B00400
#define DAC18B *(volatile u32*)0x25B00400
#define VER *(volatile u32*)0x25B00400
#define MVOL *(volatile u32*)0x25B00400

#define RBL *(volatile u32*)0x25B00402
#define RBP *(volatile u32*)0x25B00402

#define MOFULL *(volatile u32*)0x25B00404
#define MOEMP *(volatile u32*)0x25B00404
#define MIOVF *(volatile u32*)0x25B00404
#define MIFULL *(volatile u32*)0x25B00404
#define MIEMP *(volatile u32*)0x25B00404
#define MIBUF *(volatile u32*)0x25B00404

#define MOBUF *(volatile u32*)0x25B00406

#define MSLC *(volatile u32*)0x25B00408
#define CA *(volatile u32*)0x25B00408

#define DMEAL *(volatile u32*)0x25B00412

#define DMEAH *(volatile u32*)0x25B00414
#define DRGA *(volatile u32*)0x25B00414

#define DGATE *(volatile u32*)0x25B00416
#define DDIR *(volatile u32*)0x25B00416
#define DEXE *(volatile u32*)0x25B00416
#define DTLG *(volatile u32*)0x25B00416

#define TACTL *(volatile u32*)0x25B00418
#define TIMA *(volatile u32*)0x25B00418

#define TBCTL *(volatile u32*)0x25B0041A
#define TIMB *(volatile u32*)0x25B0041A

#define TCCTL *(volatile u32*)0x25B0041C
#define TIMC *(volatile u32*)0x25B0041C

#define SCIEB *(volatile u32*)0x25B0041E

#define SCIPD *(volatile u32*)0x25B00420

#define SCIRE *(volatile u32*)0x25B00422

#define SCILV0 *(volatile u32*)0x25B00424

#define SCILV1 *(volatile u32*)0x25B00426

#define SCILV2 *(volatile u32*)0x25B00428

#define MCIEB *(volatile u32*)0x25B0042A

#define MCIPD *(volatile u32*)0x25B0042C

#define MCIRE *(volatile u32*)0x25B0042E


