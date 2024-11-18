#include "p1.h"

#define SCHEDULER_CH_ID 2
#define PPD_CH_ID 5

/* PORTS */
volatile SAMPLING_PORT_TYPE *PPD_P2_PORT;
volatile SAMPLING_PORT_TYPE *PPD_BROADCAST_PORT;

volatile SAMPLING_PORT_TYPE *SEND_P2_PORT;
volatile SAMPLING_PORT_TYPE *SEND_ALL_PORT;

/* PARTITION INFO */

volatile PARTITION_SHARED_t *P_STATE;
volatile PD_STATUS_t *PPD_STATUS;

void init(void) {
    if (PPD_STATUS->status == READY) { 
        microkit_dbg_puts("P1 PPD READY, Initialising P1 SPD\n");
        P_STATE->state = READY;
        /* Set port status */
        reset_port(SEND_P2_PORT);
        reset_port(SEND_ALL_PORT);
    }
};

void notified(microkit_channel ch) {
    switch (ch) {
    /* Start of partition time slice */
    case SCHEDULER_CH_ID:
        
        /* Set default, empty and invalid */
        reset_port(SEND_P2_PORT);
        reset_port(SEND_ALL_PORT);
        reset_port(PPD_P2_PORT);
        reset_port(PPD_BROADCAST_PORT);

        /* Invoke pPD (donate scheduling context)*/
        microkit_ppcall(PPD_CH_ID, microkit_msginfo_new(0,0));

        /* Checks if there is message from PPD port, if so copies to send port and sets flags */
        check_set_message(PPD_P2_PORT, SEND_P2_PORT);
        check_set_message(PPD_BROADCAST_PORT, SEND_ALL_PORT); 
 
        /* If pPD returns, did not overrun */
        P_STATE->state = READY;
    }
};


