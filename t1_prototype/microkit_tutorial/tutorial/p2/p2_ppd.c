#include "p2_ppd.h"

#define SPD_CH_ID 6

/* Send Port */
volatile SAMPLING_PORT_TYPE *P3_PORT;
/* Receive Port */
volatile SAMPLING_PORT_TYPE *P1_RECV;
volatile SAMPLING_PORT_TYPE *P1_BROADCAST_RECV;

volatile PD_STATUS_t *STATUS;

void send_p3(int message) {
    write_to_port(P3_PORT, message);
}

int read_p1() {
    return read_from_port(P1_RECV);
}
int read_broadcast() {
    return read_from_port(P1_BROADCAST_RECV);
}

void init(void) {
    microkit_dbg_puts("Initialising P1 PPD\n");
    p2_initialize();
    STATUS->status = READY;
    reset_port(P3_PORT);
}

microkit_msginfo protected(microkit_channel channel, microkit_msginfo msginfo) {
    switch (channel) {
        case SPD_CH_ID:    
            microkit_dbg_puts("P1 PPD: Received PPC from SPD!\n");
            /* TODO: Run periodic application code */
            p2_timeTriggered();
            return microkit_msginfo_new(0,0);
    }
}

void notified(microkit_channel ch) {};