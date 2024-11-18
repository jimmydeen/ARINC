#include "p3.h"

void p3_initialize(void) {
  // add initialization code here
  printf("P3: INIT USER APPLICATION\n");
}

void p3_timeTriggered() {
  // add compute phase code here
  // printf("P3: TIME TRIGGERED\n");
  int private_val = read_p2();
  int broadcast_val = read_broadcast();

  if (private_val == broadcast_val) {
    printf("Broadcast value : %d == P2 val : %d\n", broadcast_val, private_val);
  } else if (private_val == -1) {
    printf("P3: Received Broadcast: %d\n", broadcast_val);
  } else {
    printf("P3: Received Value From P2 : %d\n", private_val);
  }
}
