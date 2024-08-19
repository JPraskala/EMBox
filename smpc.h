//
// Created by andrewpraskala on 7/11/24.
//

#ifndef SMPC_H
#define SMPC_H

#include <stdint.h>

// SMPC Memory Map (8 bit reg accessed as 32 bit)
typedef volatile uint32_t* SMPC_Register;

typedef struct {
    SMPC_Register ireg[7]; // input registers
    SMPC_Register comreg; // command register
    SMPC_Register oreg[32]; // output registers
    SMPC_Register sr; // status register
    SMPC_Register sf; // status flag
    SMPC_Register pdr[2]; // port data register
    SMPC_Register ddr[2]; // data direction register
    SMPC_Register iosel; // input/output selection register
    SMPC_Register exle; // external latch enable register
} SMPC_Registers;

#define SMPC_REGISTERS ((SMPC_Registers*)0x20100001)

// SMPC Commands (8 bits)
typedef enum {
    CMD_MSHON = 0x00,         // Reset and enable SH-2 Master CPU
    CMD_SSHON = 0x02,         // Reset and enable SH-2 Slave CPU
    CMD_SSHOFF = 0x03,        // Disable SH-2 Slave CPU
    CMD_SNDON = 0x06,         // Reset and enable Motorola 68000 (sound) CPU
    CMD_SNDOFF = 0x07,        // Disable Motorola 68000 (sound) CPU
    CMD_CDON = 0x08,          // Reset and enable CD Block
    CMD_CDOFF = 0x09,         // Disable CD Block
    CMD_NETLINKON = 0x0A,     // Reset and enable Netlink execution
    CMD_NETLINKOFF = 0x0B,    // Disable Netlink execution
    CMD_SYSRES = 0x0D,        // Reset the System
    CMD_CKCHG352 = 0x0E,      // Change system clock to 352 mode (NTSC)
    CMD_CKCHG320 = 0x0F,      // Change system clock to 320 mode
    CMD_INTBACK = 0x10,       // Fetch SMPC status and peripheral data
    CMD_SETTIME = 0x16,       // Set date and time for the RTC
    CMD_SETSMEM = 0x17,       // Set 4-byte battery-backed memory
    CMD_NMIREQ = 0x18,        // Send NMI request to Master SH2
    CMD_RESENAB = 0x19,       // Enable NMI requests on Reset button press
    CMD_RESDISA = 0x1A        // Disable NMI requests on Reset button press
} SMPC_Command;

// SMPC Errors
typedef enum {
    SMPC_ERROR_TIMEOUT = 0x01,
    SMPC_ERROR_INVALID_COMMAND = 0x02,
    SMPC_ERROR_HARDWARE_FAILURE = 0x04,
    SMPC_ERROR_INVALID_INTBACK_PARAM = 0x06
} SMPC_Error;

// Port enumeration
typedef enum {
    PORT_1 = 1,
    PORT_2 = 2
} Port;

typedef enum {
    FIFTEEN_BYTE_MODE = 15,
    TWO_HUNDRED_FIFTY_FIVE_MODE = 255
} PortMode;

// CPU cycle rate
#define CPU_CYCLES_PER_MICROSECOND 28.6363636

// Saturn RTC structure
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t dayOfTheWeek;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} SaturnRtc;

// System Initialization Operations

bool SMPC_Init(void);
void SMPC_PowerOn(void);
void SMPC_Reset(void);
void SMPC_ChangeClockTo352(void);
void SMPC_ChangeClockTo320(void);
bool SMPC_ExecuteCommand(SMPC_Command command);
void SMPC_HandleError(SMPC_Error error_code);
void SMPC_ProcessINTBACKResults(void);
uint32_t SMPC_GetPeripheralData(uint8_t port);
void SMPC_ResetCPUs(void);
void SMPC_ResetMemory(void);
void SMPC_ResetHardwareComponents(void);
void SMPC_ResetCDBlock(void);
void SMPC_ReinitializeBIOS(void);
void SMPC_ResetSystemClocksAndTimers(void);
void SMPC_ClearPendingInterrupts(void);
void SMPC_ResetIOPortsAndPeripherals(void);
void SMPC_ValidateRTCValues(uint8_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek,
                       uint8_t hour, uint8_t minute, uint8_t second);
void SMPC_HandleTime(void);
void SMPC_HandleStatusData(void);
void SMPC_ScanPeripheral(Port port, PortMode port_mode, bool optimizeAcquisitionTime);

#endif //SMPC_H