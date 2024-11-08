#include "p1_ppd.h"

#define SPD_CH_ID 5

volatile SAMPLING_PORT_TYPE *P2_PORT;
volatile SAMPLING_PORT_TYPE *BROADCAST_PORT;

volatile PD_STATUS_t *STATUS;

void send_p2(int message) {
    write_to_port(P2_PORT, message);
}

void broadcast(int message) {
    write_to_port(BROADCAST_PORT, message);
}

void init(void) {
    microkit_dbg_puts("Initialising P1 PPD\n");
    p1_initialize();
    STATUS->status = READY;
}

microkit_msginfo protected(microkit_channel channel, microkit_msginfo msginfo) {
    switch (channel) {
        case SPD_CH_ID:    
            microkit_dbg_puts("P1 PPD: Received PPC from SPD!\n");
            /* TODO: Run periodic application code */
            p1_timeTriggered();
            return microkit_msginfo_new(0,0);
    }
}

void notified(microkit_channel ch) {};