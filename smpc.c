#include "smpc.h"
#include <stdio.h>

static uint8_t smpc_initialized = 0; // 1=initialized 0=not initialized
static uint8_t current_clock_mode = 0; // 1=352(NTSC) mode 0=320(PAL) mode
static SaturnRtc saturn_rtc;
static uint8_t smpc_power = 0; // 0 = off 1 = on

int SMPC_Init(void) {

    if (smpc_initialized == 1) {
        printf("The SMPC is already initialized.\n");
        return 1;
    }

    SMPC_REGISTERS->iosel = 0x00; // Set IOSEL to manual mode for complete control over I/O
    if (SMPC_REGISTERS->iosel != 0x00) {
        printf("Failed to set IOSEL.\n");
        return 1;
    }

    SMPC_REGISTERS->ddr[0] = 0x00; // Set DDR1 to input mode
    SMPC_REGISTERS->ddr[1] = 0x00; // Set DDR2 to input mode
    if (SMPC_REGISTERS->ddr[0] != 0x00 || SMPC_REGISTERS->ddr[1] != 0x00) {
        printf("Failed to set DDR1 or DDR2 to input mode.\n");
        return 1;
    }

    SMPC_REGISTERS->exle = 0x00; // Disable the EXLE register
    if (SMPC_REGISTERS->exle != 0x00) {
        printf("Failed to set EXLE.\n");
        return 1;
    }

    // Clearing additional registers
    SMPC_REGISTERS->sr = 0x00;
    SMPC_REGISTERS->comreg = 0x00;

    // Resetting clock mode
    current_clock_mode = 0;

    // The SMPC is now initialized
    smpc_initialized = 1;
    printf("SMPC is initialized.\n");
    return 0;
}

void SMPC_PowerOn(void) {
    if (smpc_power == 0) {
        printf("SMPC is already powered on.\n");
    }
    SMPC_ExecuteCommand(CMD_MSHON); // turn master cpu on
    SMPC_ExecuteCommand(CMD_SSHON); // turn slave cpu on
    SMPC_ExecuteCommand(CMD_SNDON); // turn sound on
    SMPC_ExecuteCommand(CMD_CDON); // turn cd block on
    smpc_power = 1;
    printf("SMPC is powered on.\n");
}

void SMPC_PowerOff(void) {
    if (smpc_power == 0) {
        printf("SMPC is already powered off.\n");
    }
    SMPC_ExecuteCommand(CMD_SSHOFF); // turn slave cpu off
    SMPC_ExecuteCommand(CMD_SNDOFF); // turn sound off
    SMPC_ExecuteCommand(CMD_CDOFF); // turn cd block off
    smpc_power = 0;
    printf("SMPC is powered off.\n");
}

void SMPC_Reset(void) {
    SMPC_ExecuteCommand(CMD_SYSRES);
    printf("SMPC reset completed.\n");
}


void SMPC_ChangeClockTo352(void) {
    if (current_clock_mode == 1) {
        return;  // Already in 352 mode
    }
    SMPC_ExecuteCommand(CMD_CKCHG352);
    current_clock_mode = 1;
    printf("Clock change to 352 mode.\n");
}

void SMPC_ChangeClockTo320(void) {
    if (current_clock_mode == 0) {
        return;  // Already in 320 mode
    }
    SMPC_ExecuteCommand(CMD_CKCHG320);
    current_clock_mode = 0;
    printf("Clock changed to 320 mode.\n");
}

int SMPC_ExecuteCommand(uint32_t* command) {
    SMPC_REGISTERS->comreg = command;
    uint32_t status = 1;
    *(SMPC_REGISTERS->sf) = status;
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
            SMPC_ProcessINTBACKResults();
            break;

        case CMD_SETTIME:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            SMPC_HandleTime();
            break;

        case CMD_SETSMEM:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            // Set 4-byte battery-backed memory based on IREG values
            break;

        case CMD_NMIREQ:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            SMPC_NMIReq();
            break;

        case CMD_RESENAB:
        case CMD_RESDISA:
            cycles_to_execute = (uint64_t)(30 * CPU_CYCLES_PER_MICROSECOND);
            // Enable/Disable NMI requests on Reset button press
            break;

        default:
            SMPC_HandleError(SMPC_ERROR_INVALID_COMMAND);
            return 1;
    }

    // cannot exceed timeout value
    int accum = 0;
    while (cycles_to_execute > accum) {
        accum++;
    }

    printf("SMPC command 0x%02X executed successfully.\n", (uint8_t)command);
    // command is done processing
    *(SMPC_REGISTERS->sr) |= 0x01; // set first bit to 1
    status = 0;
    *(SMPC_REGISTERS->sf) = status; // done processing
    return 0;
}

void HandleTime(void) {
    uint16_t year = *(SMPC_REGISTERS->ireg[0]) & 0xFFFF; // 4 digit year
    uint8_t month = *(SMPC_REGISTERS->ireg[1]) & 0xFF;    // Month
    uint8_t day = *(SMPC_REGISTERS->ireg[2]) & 0xFF;      // Day
    uint8_t dayOfWeek = *(SMPC_REGISTERS->ireg[3]) & 0x7; // Day of week (0-6, where 0 = Sunday)
    uint8_t hour = *(SMPC_REGISTERS->ireg[4]) & 0xFF;     // Hour
    uint8_t minute = *(SMPC_REGISTERS->ireg[5]) & 0xFF;   // Minute
    uint8_t second = *(SMPC_REGISTERS->ireg[6]) & 0xFF;   // Second

    if (year > 9999) year = 9999;
    if (month == 0 || month > 12) month = 12;
    if (day == 0 || day > 31) day = 31;
    if (dayOfWeek > 6) dayOfWeek = 6;
    if (hour > 23) hour = 23;
    if (minute > 59) minute = 59;
    if (second > 59) second = 59;

    saturn_rtc.year = year;
    saturn_rtc.month = month;
    saturn_rtc.day = day;
    saturn_rtc.dayOfTheWeek = dayOfWeek;
    saturn_rtc.hour = hour;
    saturn_rtc.minute = minute;
    saturn_rtc.second = second;

    *(SMPC_REGISTERS->oreg[31]) = saturn_rtc.year;
    *(SMPC_REGISTERS->oreg[30]) = saturn_rtc.day << 4 | saturn_rtc.dayOfTheWeek;
    *(SMPC_REGISTERS->oreg[29]) = saturn_rtc.hour;
    *(SMPC_REGISTERS->oreg[28]) = saturn_rtc.minute;
    *(SMPC_REGISTERS->oreg[27]) = saturn_rtc.second;
    *(SMPC_REGISTERS->oreg[26]) = saturn_rtc.month;
}

void SMPC_ProcessINTBACKResults(void) {
    const uint8_t firstParam = *(SMPC_REGISTERS->ireg[0]);
    const uint8_t secondParam = *(SMPC_REGISTERS->ireg[1]);
    const uint8_t thirdParam = *(SMPC_REGISTERS->ireg[2]);
    if (thirdParam != 0xF0) {
        SMPC_HandleError(SMPC_ERROR_INVALID_INTBACK_PARAM);
        return;
    }
    if (firstParam == 0x01) {
        SMPC_HandleStatusData();
    } else if(firstParam == 0x00 || firstParam == 0x02) {
        const uint8_t acquisitionTime_IsOptimized = (secondParam & 0x01) == 0;
        const uint8_t peripheralData_IsEnabled = (secondParam & 0x04) == 0;
        const uint8_t port1_15ByteMode = ((secondParam >> 6) & 0x00) == 0;
        const uint8_t port1_255ByteMode = ((secondParam >> 6) & 0x04) == 0;
        const uint8_t port2_15ByteMode = ((secondParam >> 4) & 0x00) == 0;
        const uint8_t port2_255ByteMode = ((secondParam >> 4) & 0x04) == 0;
        if (peripheralData_IsEnabled) {
            enum Port port;
            enum PortMode port_mode;
            if (port1_15ByteMode) {
                port = PORT_1;
                port_mode = FIFTEEN_BYTE_MODE;
                SMPC_ScanPeripheral(port, port_mode, acquisitionTime_IsOptimized);
            } else if (port1_255ByteMode) {
                port = PORT_1;
                port_mode = TWO_HUNDRED_FIFTY_FIVE_MODE;
                SMPC_ScanPeripheral(port, port_mode, acquisitionTime_IsOptimized);
            }

            if (port2_15ByteMode) {
                port = PORT_2;
                port_mode = FIFTEEN_BYTE_MODE;
                SMPC_ScanPeripheral(port, port_mode, acquisitionTime_IsOptimized);
            } else if (port2_255ByteMode) {
                port = PORT_2;
                port_mode = TWO_HUNDRED_FIFTY_FIVE_MODE;
                SMPC_ScanPeripheral(port, port_mode, acquisitionTime_IsOptimized);
            } else {
                printf("Invalid Mode\n");
            }
        }
    } else {
        printf("Break request\n");
    }
    *(SMPC_REGISTERS->sr) |= 0x01;
}

void SMPC_MSHON() {
    *(SMPC_REGISTERS->oreg[31]) = 0x00;
}

void SMPC_SSHON() {
    *(SMPC_REGISTERS->oreg[31]) = 0x02;
}

void SMPC_SSHOFF() {
    *(SMPC_REGISTERS->oreg[31]) = 0x03;
}

void SMPC_SNDON() {
    *(SMPC_REGISTERS->oreg[31]) = 0x06;
}

void SMPC_SNDOFF() {
    *(SMPC_REGISTERS->oreg[31]) = 0x7;
}

void SMPC_CDON() {
    *(SMPC_REGISTERS->oreg[31]) = 0x08;
}

void SMPC_CDOFF() {
    *(SMPC_REGISTERS->oreg[31]) = 0x09;
}

void SMPC_SYSRES() {
    *(SMPC_REGISTERS->oreg[31]) = 0x0D;
}

void SMPC_CKCHG352() {
    *(SMPC_REGISTERS->oreg[31]) = 0x0E;
}

void SMPC_CKCHG320() {
    *(SMPC_REGISTERS->oreg[31]) = 0x0F;
}

void SMPC_NMIReq() {
    // send interrupt
    *(SMPC_REGISTERS->oreg[31]) = 0x18;
}

void SMPC_RESENAB() {
    *(SMPC_REGISTERS->oreg[31]) = 0x19;
}

void SMPC_RESDISA() {
    *(SMPC_REGISTERS->oreg[31]) = 0x1A;
}

void SMPC_HandleError(const uint32_t error_code) {
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
