//
// Created by andrewpraskala on 7/11/24.
//

#ifndef SMPC_H
#define SMPC_H

#include <stdint.h>

// SMPC Memory Map (8 bit reg accessed as 32 bit)

/*
 * 32 bit casting due to Saturn's bus = 32 bits wide
 *
 * The registers themselves are 8 bits but are accessed via a 32-bit bus
 */

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

// SMPC Commands (8 bits)

/*
 * the commands are 8 bit values: their actual size in the SMPC hardware
 *
 * only the least significant byte is relevant
 * note: each digit in a hexadecimal represents 4 bits
 */

// Reset and enable SH-2 Master CPU
#define CMD_MSHON 0x00

// Reset and enable SH-2 Slave CPU
#define CMD_SSHON 0x02

// Disable SH-2 Slave CPU
#define CMD_SSHOFF 0x03

// Reset and enable Motorola 68000 (sound) CPU
#define CMD_SNDON 0x06

// Disable Motorola 68000 (sound) CPU
#define CMD_SNDOFF 0x07

// Reset and enable CD Block
#define CMD_CDON 0x08

// Disable CD Block
#define CMD_CDOFF 0x09

// Reset and enable Netlink execution
#define CMD_NETLINKON 0x0A

// Disable Netlink execution
#define CMD_NETLINKOFF 0x0B

// Reset the System
#define CMD_SYSRES 0x0D

// Change system clockspeed (352)
#define CMD_CKCHG352 0x0E

// Change system clockspeed (320)
#define CMD_CKCHG320 0x0F

// Fetch SMPC status and peripheral data
#define CMD_INTBACK 0x10

// Set date and time for the RTC
#define CMD_SETTIME 0x16

// Set 4-byte battery-backed memory
#define CMD_SETSMEM 0x17

// Send NMI request to Master SH2
#define CMD_NMIREQ 0x18

// Enable NMI requests on Reset button press
#define CMD_RESENAB 0x19

// Disable NMI requests on Reset button press
#define CMD_RESDISA 0x1A

// System Initialization Operations

// Initialize the SMPC subsystem
/*
 * Sets up initial values for the registers.
 * Configures interrupt handlers
 * The first operation
 */
void SMPC_Init(void);

// Execute the power-on
/*
 * Enables the master SH-2 CPU (by using the CMD_MSHON)
 * This simulates the power-on seq -- orders functions by a hierarchy
 * Sets up the "initial" states of things such as clock speed
 */
void SMPC_PowerOn(void);

// Execute the system reset sequence
/*
 * Simulates when you press the "reset" button on the console (which calls the CMD_SYSRES)
 */
void SMPC_Reset(void);

// Change system clock to 352 mode
/*
 * Executes the CMD_CKCHG352 command to change the system clock to 352 mode
 * This is for NTSC (National Television Standards Committee) video mode
 */
void SMPC_ChangeClockTo352(void);

// Change system clock to 320 mode
/*
 * Changes the system clock to 320 mode by executing CMD_CKCHG320
 */
void SMPC_ChangeClockTo320(void);

// General command execution function
/*
 * A utility function to send commands to the SMPC
 * 1. It will write commands to the COMREG
 * 2. It will wait for a status of completion (by checking the SF flag)
 */
void SMPC_ExecuteCommand(uint32_t command);

// Handles errors
void SMPC_HandleError(uint32_t error_code);

// Scans all connected peripherals
/*
 * Sends the INTBACK command to the SMPC to display status
 */
void SMPC_ScanPeripherals(void);

// Retrieves the data for a specific peripheral port
/*
 * Cannot exceed two ports ( 0>n<=2 where n is a whole number )
 */
uint32_t SMPC_GetPeripheralData(uint8_t port);

// Retrieves the current time from the RTC (real-time clock)
/*
 * CONSIDERATION: Saturn's RTC utilizes a 2-digit year... We need to handle the century
 *

Sega Saturn uses BCD for date format

BCD = Binary Coded Decimal

Year: 00-99 (BCD)
Month: 01-12 (BCD)
Day: 01-31 (BCD)
Hour: 00-23 (BCD)
Minute: 00-59 (BCD)
Second: 00-59 (BCD)
 */
void SMPC_GetTime(uint8_t *year, uint8_t *month, uint8_t *day,
                  uint8_t *hour, uint8_t *minute, uint8_t *second);


// Exceptions

#define SMPC_TIMEOUT_VALUE 1000
#define SMPC_ERROR_TIMEOUT 0x01
#define SMPC_ERROR_INVALID_COMMAND 0x02
#define SMPC_ERROR_HARDWARE_FAILURE 0x04

#endif //SMPC_H
