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
            // Send NMI request to Master SH2
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



