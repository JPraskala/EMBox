//
// Created by andrewpraskala on 7/11/24.
//

#include "smpc.h"
#include <stdio.h>

// to keep track of SMPC's internal state across function calls
// this can reduce redundant reseting operations to re-initailize or change to the same clock mode
static uint8_t smpc_initialized = 0; // 1=initialized 0=not initialized
static uint8_t current_clock_mode = 0; // 1=352(NTSC) mode 0=320(PAL) mode
static SaturnRtc saturn_rtc;

// initialize SMPC
bool SMPC_Init(void) {
    if (smpc_initialized) {
        printf("SMPC already initialized.\n");
        return false; // avoiding re-initialization
    }

    // IOSEL set to manual mode for complete control over i/o (automatic would make too many assumptions)
    IOSEL = 0x00;
    if (IOSEL != 0x00) {
        printf("Failed to set IOSEL.\n");
        return false;
    }

    // Setting the DDR1 and DDR2 registers to 0x00 for Input Mode (since we may not know what is connected to the ports)
    // 0x00 configures all i/o ports as inputs -- which leads to peripheral detection
    DDR1 = 0x00;
    DDR2 = 0x00;
    if (DDR1 != 0x00 || DDR2 != 0x00) {
        printf("Failed to set DDR1 or DDR2 to input mode.\n");
        return false;
    }

    // disabling the EXLE register
    EXLE = 0x00;
    if (EXLE != 0x00) {
        printf("Failed to set EXLE.\n");
        return false;
    }

    // clearing additional registers
    SR = 0x00;
    COMREG = 0x00;

    // reseting clock mode
    current_clock_mode = 0;

    // the SMPC is now initialized
    smpc_initialized = 1;
    printf("SMPC is initialized.\n");
    return true;
}

void SMPC_PowerOn(void) {
    // enabling master SH-2 CPU
    SMPC_ExecuteCommand(CMD_MSHON);
    printf("SMPC is powered on.\n");
}

void SMPC_Reset(void) {
    // reseting the system
    SMPC_ExecuteCommand(CMD_SYSRES);
    printf("SMPC reset completed.\n");
}

void SMPC_ChangeClockTo352(void) {
    if (current_clock_mode) {
        return;  // Already in 352 mode
    }
    SMPC_ExecuteCommand(CMD_CKCHG352);
    current_clock_mode = 1;
    printf("Clock change to 352 mode.\n");
}

void SMPC_ChangeClockTo320(void) {
    if (!current_clock_mode) {
        return;  // Already in 320 mode
    }
    SMPC_ExecuteCommand(CMD_CKCHG320);
    current_clock_mode = 0;
    printf("Clock changed to 320 mode.\n");
}

/*
 * Takes a command (such as CMD_MSHON, CMD_SSHON) as a parameter
 * Writes the command to the COMREG (command reg)
 * Writing to the COMREG triggers the SMPC to start executing the command
 * */
bool SMPC_ExecuteCommand(uint32_t command) {
    COMREG = command;
    SF = 1;
    uint32_t cycles_to_execute;

    switch(command) {
        case CMD_MSHON:
        case CMD_SSHON:
        case CMD_SSHOFF:
        case CMD_SNDON:
        case CMD_SNDOFF:
        case CMD_CDON:
        case CMD_CDOFF:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            break;

        case CMD_NETLINKON:
        case CMD_NETLINKOFF:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            break;

        case CMD_SYSRES:
            cycles_to_execute = (uint64_t)(100000 * CPU_CYCLES_PER_MICROSECOND);
            SMPC_ResetCPUs();
            SMPC_ResetMemory();
            SMPC_ResetHardwareComponents();
            SMPC_ResetCDBlock();
            SMPC_ReinitializeBIOS();
            SMPC_ResetSystemClocksAndTimers();
            SMPC_ClearPendingInterrupts();
            SMPC_ResetIOPortsAndPeripherals();
            break;

        case CMD_CKCHG352:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            current_clock_mode = 1;
            break;

        case CMD_CKCHG320:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            current_clock_mode = 0;
            break;

        case CMD_INTBACK:
            cycles_to_execute = (uint64_t)(320 * CPU_CYCLES_PER_MICROSECOND);
            ProcessINTBACKResults();
            break;

        case CMD_SETTIME:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            HandleTime();
            break;

        case CMD_SETSMEM:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            // Set 4-byte battery-backed memory based on IREG values
            break;

        case CMD_NMIREQ:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            // Send NMI request to Master SH2
            break;

        case CMD_RESENAB:
        case CMD_RESDISA:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            // Enable/Disable NMI requests on Reset button press
            break;

        default:
            SMPC_HandleError(SMPC_ERROR_INVALID_COMMAND);
            return false;
    }

    // cannot exceed timeout value
    while (cycles_to_execute > 0) {
        cycles_to_execute--;
    }

    printf("SMPC command 0x%02X executed successfully.\n", (uint8_t)command);
    // command is done processing
    SR |= 0x01; // set first bit to 1
    SF = 0; // done processing
    return true;
}

void SMPC_HandleError(uint32_t error_code) {
    switch(error_code) {
        case SMPC_ERROR_TIMEOUT:
            printf("SMPC Error: Command timeout\n");
        break;
        case SMPC_ERROR_INVALID_COMMAND:
            printf("SMPC Error: Invalid command\n");
        break;
        case SMPC_ERROR_HARDWARE_FAILURE:
            printf("SMPC Error: Hardware failure\n");
        break;
        case SMPC_ERROR_INVALID_INTBACK_PARAM:
            printf("SMPC Error: Intback parameter 2 not properly set to F0H\n");
            break;
        default:
            printf("SMPC Error: Unknown error code %u\n", error_code);
    }
}

void HandleTime(void) {
    uint8_t year = IREG0 & 0xFF;     // Last two digits of year
    uint8_t month = IREG1 & 0xFF;    // Month
    uint8_t day = IREG2 & 0xFF;      // Day
    uint8_t dayOfWeek = IREG3 & 0x7; // Day of week (0-6, where 0 = Sunday)
    uint8_t hour = IREG4 & 0xFF;     // Hour
    uint8_t minute = IREG5 & 0xFF;   // Minute
    uint8_t second = IREG6 & 0xFF;   // Second

    if (year > 0x99) year = 0x99;
    if (month == 0 || month > 0x12) month = 0x01;
    if (day == 0 || day > 0x31) day = 0x01;
    if (dayOfWeek > 6) dayOfWeek = 0;
    if (hour > 0x23) hour = 0x00;
    if (minute > 0x59) minute = 0x00;
    if (second > 0x59) second = 0x00;

    saturn_rtc.year = year;
    saturn_rtc.month = month;
    saturn_rtc.day = day;
    saturn_rtc.dayOfTheWeek = dayOfWeek;
    saturn_rtc.hour = hour;
    saturn_rtc.minute = minute;
    saturn_rtc.second = second;

    OREG31 = saturn_rtc.year;
    // Day in upper 4 bits, Day of week in lower 4 bits
    OREG30 = saturn_rtc.day << 4 | saturn_rtc.dayOfTheWeek;
    OREG29 = saturn_rtc.hour;
    OREG28 = saturn_rtc.minute;
    OREG27 = saturn_rtc.second;
    OREG26 = saturn_rtc.month;
}

void ProcessINTBACKResults(void) {
    uint8_t firstParam = IREG0;
    uint8_t secondParam = IREG1;
    // signal register
    uint8_t thirdParam = IREG2;

    // IREG2 not properly set
    if (thirdParam != 0xF0) {
        SMPC_HandleError(SMPC_ERROR_INVALID_INTBACK_PARAM);
        return;
    }


    // just get smpc status
    if (firstParam == 0x01) {
        HandleStatusData();
    // either just get peripheral data or both
    } else if (firstParam == 0x00 || firstParam == 0x02) {
        // parsing the IREG0
        bool optimizeAcquisitionTime = (secondParam & 0x01) != 0; // Bit 0
        bool getPeripheralData = (secondParam & 0x02) != 0; // Bit 1
        bool getPort1Data = (secondParam & 0x04) != 0; // Bit 2
        bool getPort2Data = (secondParam & 0x10) != 0; // Bit 4

        uint8_t port1Mode = (secondParam >> 4) & 0x03; // Bits 5-4 (masking via 0x11)
        uint8_t port2Mode = (secondParam >> 6) & 0x03; // Bits 7-6 (masking via 0x11)
        if (getPeripheralData) {
            enum Port port;
            enum PortMode port_mode;
            if (getPort1Data) {
                switch (port1Mode) {
                    case 0:
                        port = PORT_1;
                        port_mode = FIFTEEN_BYTE_MODE;
                        ScanPeripheral(port, port_mode, optimizeAcquisitionTime);
                        break;
                    case 1:
                        port = PORT_1;
                        port_mode = TWO_HUNDRED_FIFTY_FIVE_MODE;
                        ScanPeripheral(port, port_mode, optimizeAcquisitionTime);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        break;
                }
            }
            if (getPort2Data) {
                switch (port2Mode) {
                    case 0:
                        port = PORT_2;
                        port_mode = FIFTEEN_BYTE_MODE;
                        ScanPeripheral(port, port_mode, optimizeAcquisitionTime);
                        break;
                    case 1:
                        port = PORT_2;
                        port_mode = TWO_HUNDRED_FIFTY_FIVE_MODE;
                        ScanPeripheral(port, port_mode, optimizeAcquisitionTime);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        break;;
                }
            }
        }
    }
    SR = 0x01;
    SR |= 0x01;
}

