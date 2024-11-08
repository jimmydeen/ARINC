#include <printf.h>
#include "p1_user.h"

int value = 0;
int broadcast_val = 1000;

void p1_initialize(void) {
  // add initialization code here
  send_p2(value);
  broadcast(broadcast_val);
  printf("P1: INIT USER APPLICATION\n");
  
}

void p1_timeTriggered() {
  // add compute phase code here
  printf("P1: TIME TRIGGERED\n");
  ++value;
  --broadcast_val;
  send_p2(value);
  broadcast(broadcast_val);
}
