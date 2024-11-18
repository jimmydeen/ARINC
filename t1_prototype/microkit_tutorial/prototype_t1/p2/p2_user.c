#include "p2.h"

void p2_initialize(void) {
  // add initialization code here
  printf("P2: INIT USER APPLICATION\n");
}

void p2_timeTriggered() {
  // add compute phase code here
  // printf("P2: TIME TRIGGERED\n");
  int privateval = read_p1();
  if (privateval % 2 == 0) {
    send_p3(privateval);
    printf("Allowed even number : %d\n", privateval);
  } else {
    printf("Blocked odd number : %d\n", privateval);
  }
}
