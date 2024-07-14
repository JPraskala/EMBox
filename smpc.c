//
// Created by andrewpraskala on 7/11/24.
//

#include "smpc.h"
#include <stdio.h>

// to keep track of SMPC's internal state across function calls
// this can reduce redundant reseting operations to re-initailize or change to the same clock mode
static uint8_t smpc_initialized = 0; // 1=initialized 0=not initialized
static uint8_t current_clock_mode = 0; // 1=352(NTSC) mode 0=320(PAL) mode

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
 *
 * The timeout is needed to prevent infinite waiting (if it reaches zero from 1000 -- call the exception)
 * If the timeout = 0 before the SF is cleared to 0 then the command did not complete in time
 */
bool SMPC_ExecuteCommand(uint32_t command) {
    COMREG = command;

    uint32_t timeout = SMPC_TIMEOUT_VALUE;
    while (SF && timeout > 0) {
        timeout--;
    }

    if (timeout == 0) {
        SMPC_HandleError(SMPC_ERROR_TIMEOUT);
        return false;
    }

    printf("SMPC command 0x%02X executed successfully.\n", (uint8_t)command);
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
        default:
            printf("SMPC Error: Unknown error code %u\n", error_code);
    }
}

