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
void SMPC_Init(void) {
    if (smpc_initialized) {
        return; // avoiding re-initialization
    }


    // IOSEL set to manual mode for complete control over i/o (automatic would make too many assumptions)
    IOSEL = 0x00;

    // Setting the DDR1 and DDR2 registers to 0x00 for Input Mode (since we may not know what is connected to the ports)
    // 0x00 configures all i/o ports as inputs -- which leads to peripheral detection

    // the SMPC is now initialized
    smpc_initialized = 1;
    printf("SMPC initialized.\n");
}

