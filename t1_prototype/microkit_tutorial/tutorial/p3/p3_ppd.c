#include "p3_ppd.h"

#define SPD_CH_ID 7
volatile PD_STATUS_t *STATUS;

/* RECEIVE PORT */
volatile SAMPLING_PORT_TYPE *P2_RECV;
volatile SAMPLING_PORT_TYPE *P1_BROADCAST_RECV;

int read_p2() {
    return read_from_port(P2_RECV);
}

int read_broadcast() {
    return read_from_port(P1_BROADCAST_RECV);
}

void init(void) {
    microkit_dbg_puts("Initialising P1 PPD\n");
    p3_initialize();
    STATUS->status = READY;
}

microkit_msginfo protected(microkit_channel channel, microkit_msginfo msginfo) {
    switch (channel) {
        case SPD_CH_ID:    
            microkit_dbg_puts("P1 PPD: Received PPC from SPD!\n");
            /* TODO: Run periodic application code */
            p3_timeTriggered();
            return microkit_msginfo_new(0,0);
    }
}

void notified(microkit_channel ch) {};