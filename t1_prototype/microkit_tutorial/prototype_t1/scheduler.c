#include "partition.h"
#include <stdint.h>
#include <microkit.h>
#include <sddf/timer/client.h>
#include <sddf/util/printf.h>

#define TIMER_CH_ID 1
#define P1_SPD_CH_ID 2
#define P2_SPD_CH_ID 3
#define P3_SPD_CH_ID 4

#define NUM_PARTITIONS 3

/*
    MR MAPPING

    1_000_000 - p1_state
    1_001_000 - p1_attr
    1_002_000 - p2_state
    1_003_000 - p2_attr
    1_004_000 - p3_state
    1_005_000 - p3_attr

*/


/* Init */




/* Partition specific */
#define P1_LEN 100 * NS_IN_MS
#define P2_LEN 100 * NS_IN_MS
#define P3_LEN 100 * NS_IN_MS

volatile PARTITION_SHARED_t *p1_state;
volatile PARTITION_SHARED_t *p2_state;
volatile PARTITION_SHARED_t *p3_state;

PARTITION_ATTR_t *p1_attr;
PARTITION_ATTR_t *p2_attr;
PARTITION_ATTR_t *p3_attr;

volatile PARTITION_SHARED_t *partition_state_list[NUM_PARTITIONS] = {0};

/* Partition scheduling management */
PARTITION_ATTR_t *partition_info_list[NUM_PARTITIONS] = {0};
uint64_t plist_head = 0;

uint64_t get_next_partition_idx() {
    // plist_head = (plist_head + 1) % NUM_PARTITIONS;
    // return plist_head;
    return plist_head++ % NUM_PARTITIONS;
}

/* Current time allocated for initialisation timeout */
uint64_t curr_init = 0;
/* Amount to decrement every time initialisation isn't complete */
uint64_t init_decrement = 5;

void init(void) {
    /* Assign partitions to list */
    partition_state_list[0] = p1_state;
    partition_state_list[1] = p2_state;
    partition_state_list[2] = p3_state;

    /* Set partitions states / info */
    for (int i = 0; i < NUM_PARTITIONS; ++i) {
        set_partition_state(partition_state_list[i], INIT);
    }

    p1_attr->LENGTH = P1_LEN;
    p1_attr->RELATIVE_START = 0;
    p2_attr->LENGTH = P2_LEN;
    p2_attr->RELATIVE_START = P1_LEN;
    p3_attr->LENGTH = P3_LEN;
    p3_attr->RELATIVE_START = P2_LEN;

    partition_info_list[0] = p1_attr;
    partition_info_list[1] = p2_attr;
    partition_info_list[2] = p3_attr;

    /* Set init completion time */
    /* Let's assume that the optimistic init time is 5 x the same as partitions time slice */
    curr_init = (P1_LEN + P2_LEN + P3_LEN);

    /* Register timer interrupt for init */
    // microkit_dbg_puts("Setting timeout\n");
    // microkit_mr_set(0, curr_init);
    // microkit_ppcall(TIMER_CH_ID, microkit_msginfo_new(0, 1));
    sddf_timer_set_timeout(TIMER_CH_ID, curr_init);
    
}

void notified(microkit_channel ch)
{
    switch (ch) {
    case TIMER_CH_ID:
        // microkit_dbg_puts("Received notification from timer driver!\n");

        /* Initialisation not complete */
        if (p1_state->state != READY || p2_state->state != READY || p3_state-> state != READY) {
            microkit_dbg_puts("INITIALISATION NOT COMPLETE. WAITING FOR INITIALISATION... \n");
            if (curr_init - init_decrement > curr_init) {
                microkit_dbg_puts("Ran out of init time?\n");
            }
            curr_init = curr_init - init_decrement;
            sddf_timer_set_timeout(TIMER_CH_ID, curr_init);
        } else {
        /* Normal operation*/
            // microkit_dbg_puts("NORMAL SCHEDULE OPERATION\n");
            int next_partition = get_next_partition_idx();
            /* TODO: Notify sPD notification */
            partition_state_list[next_partition]->state = RUNNING;
            switch (next_partition) {
                case 0:
                    // microkit_dbg_puts("NOTIFYING P1\n");
                    microkit_notify(P1_SPD_CH_ID);
                    break;
                case 1:
                    // microkit_dbg_puts("NOTIFYING P2\n");
                    microkit_notify(P2_SPD_CH_ID);
                    break;
                case 2:
                    // microkit_dbg_puts("NOTIFYING P3\n");
                    microkit_notify(P3_SPD_CH_ID);
                    break;
            }
            // microkit_ppcall(TIMER_CH_ID, microkit_msginfo_new(0, 1));
            // sddf_timer_set_timeout(TIMER_CH_ID, partition_info_list[next_partition]->LENGTH);
            sddf_timer_set_timeout(TIMER_CH_ID, curr_init);
        }
        
        /* Register next */
        break;
    default:
        microkit_dbg_puts("TIMER|ERROR: unexpected channel!\n");
    }
}