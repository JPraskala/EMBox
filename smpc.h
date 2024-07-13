//
// Created by andrewpraskala on 7/11/24.
//

#ifndef SMPC_H
#define SMPC_H

#include <stdint.h>

// MEMORY MAP

// input registers
#define IREG0 *(volatile uint32_t*)0x20100001
#define IREG1 *(volatile uint32_t*)0x20100003
#define IREG2 *(volatile uint32_t*)0x20100005
#define IREG3 *(volatile uint32_t*)0x20100007
#define IREG4 *(volatile uint32_t*)0x20100009
#define IREG5 *(volatile uint32_t*)0x2010000B
#define IREG6 *(volatile uint32_t*)0x2010000D

// command register
#define COMREG *(volatile uint32_t*)0x2010001F

// ouput registers (output for the command register)
#define OREG0   *(volatile uint32_t*)0x20100021
#define OREG1   *(volatile uint32_t*)0x20100023
#define OREG2   *(volatile uint32_t*)0x20100025
#define OREG3   *(volatile uint32_t*)0x20100027
#define OREG4   *(volatile uint32_t*)0x20100029
#define OREG5   *(volatile uint32_t*)0x2010002B
#define OREG6   *(volatile uint32_t*)0x2010002D
#define OREG7   *(volatile uint32_t*)0x2010002F
#define OREG8   *(volatile uint32_t*)0x20100031
#define OREG9   *(volatile uint32_t*)0x20100033
#define OREG10  *(volatile uint32_t*)0x20100035
#define OREG11  *(volatile uint32_t*)0x20100037
#define OREG12  *(volatile uint32_t*)0x20100039
#define OREG13  *(volatile uint32_t*)0x2010003B
#define OREG14  *(volatile uint32_t*)0x2010003D
#define OREG15  *(volatile uint32_t*)0x2010003F
#define OREG16  *(volatile uint32_t*)0x20100041
#define OREG17  *(volatile uint32_t*)0x20100043
#define OREG18  *(volatile uint32_t*)0x20100045
#define OREG19  *(volatile uint32_t*)0x20100047
#define OREG20  *(volatile uint32_t*)0x20100049
#define OREG21  *(volatile uint32_t*)0x2010004B
#define OREG22  *(volatile uint32_t*)0x2010004D
#define OREG23  *(volatile uint32_t*)0x2010004F
#define OREG24  *(volatile uint32_t*)0x20100051
#define OREG25  *(volatile uint32_t*)0x20100053
#define OREG26  *(volatile uint32_t*)0x20100055
#define OREG27  *(volatile uint32_t*)0x20100057
#define OREG28  *(volatile uint32_t*)0x20100059
#define OREG29  *(volatile uint32_t*)0x2010005B
#define OREG30  *(volatile uint32_t*)0x2010005D
#define OREG31  *(volatile uint32_t*)0x2010005F

// status register
#define SR *(volatile uint32_t*)0x20100061

// status flag (shows the status of the smpc command)
// it is usually given a '1' then cleared when it is finishing executing
#define SF *(volatile uint32_t*)0x20100063

// port data registers for i/o ports
#define PDR1 *(volatile uint32_t*)0x20100075
#define PDR2 *(volatile uint32_t*)0x20100077

// data direction registers for reading/writing from pdr
#define DDR1 *(volatile uint32_t*)0x20100079
#define DDR2 *(volatile uint32_t*)0x2010007B

// determines control (automatic or manual) for i/o
#define IOSEL *(volatile uint32_t*)0x2010007D

#define EXLE *(volatile uint32_t*)0x2010007F

// COMMANDS

// SMPC Command Macros

// Reset and enable SH-2 Master CPU
#define CMD_MSHON      0x00000000

// Reset and enable SH-2 Slave CPU
#define CMD_SSHON      0x00000002

// Disable SH-2 Slave CPU
#define CMD_SSHOFF     0x00000003

// Reset and enable Motorola 68000 (sound) CPU
#define CMD_SNDON      0x00000006

// Disable Motorola 68000 (sound) CPU
#define CMD_SNDOFF     0x00000007

// Reset and enable CD Block
#define CMD_CDON       0x00000008

// Disable CD Block
#define CMD_CDOFF      0x00000009

// Reset and enable Netlink execution
#define CMD_NETLINKON  0x0000000A

// Disable Netlink execution
#define CMD_NETLINKOFF 0x0000000B

// Reset the System
#define CMD_SYSRES     0x0000000D

// Change system clockspeed (352)
#define CMD_CKCHG352   0x0000000E

// Change system clockspeed (320)
#define CMD_CKCHG320   0x0000000F

// Fetch SMPC status and peripheral data
#define CMD_INTBACK    0x00000010

// Set date and time for the RTC
#define CMD_SETTIME    0x00000016

// Set 4-byte battery-backed memory
#define CMD_SETSMEM    0x00000017

// Send NMI request to Master SH2
#define CMD_NMIREQ     0x00000018

// Enable NMI requests on Reset button press
#define CMD_RESENAB    0x00000019

// Disable NMI requests on Reset button press
#define CMD_RESDISA    0x0000001A

#endif //SMPC_H
