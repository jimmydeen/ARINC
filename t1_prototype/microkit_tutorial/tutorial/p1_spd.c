#include "partition.h"
#include <microkit.h>

#define PPD_CH_ID 1

volatile PARTITION_SHARED_t *P_STATE;
volatile PD_STATUS_t *PPD_STATUS;

void init(void) {
    if (PPD_STATUS->status == READY) { 
        microkit_dbg_puts("P1 PPD READY, Initialising P1 SPD\n");
        P_STATE->state = READY;
    }
};

void notified(microkit_channel ch) {

};