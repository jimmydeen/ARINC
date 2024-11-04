#include <microkit.h>
#include "partition.h"

#define SPD_CH_ID 2
volatile PD_STATUS_t *STATUS;

void init(void) {
    microkit_dbg_puts("Initialising P1 PPD\n");
    STATUS->status = READY;
}

microkit_msginfo protected(microkit_channel channel, microkit_msginfo msginfo) {
    switch (channel) {
        case SPD_CH_ID:    
            microkit_dbg_puts("P1 PPD: Received PPC from SPD!\n");
            /* TODO: Run periodic application code */
            break;
    }
}

void notified(microkit_channel ch) {};