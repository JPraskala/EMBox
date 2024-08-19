#include "smpc.h"
#include <stdio.h>

static uint8_t smpc_initialized = 0; // 1=initialized 0=not initialized
static uint8_t current_clock_mode = 0; // 1=352(NTSC) mode 0=320(PAL) mode
static SaturnRtc saturn_rtc;

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
        return false;
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