/* ts_init.h - CS503 15Fall, Lab 2 - Part B */

#ifndef __TS_INIT_H_
#define __TS_INIT_H_

#include <lab2b.h>

struct ts_ent {
  int ts_tqexp;    // new priority: CPU-intensive (time quantum expired)
  int ts_slpret;   // new priority: I/O-intensive (sleep return)
  int ts_quantum;  // new time slice
};
extern struct ts_ent tstab[];

/* LAB2BTODO: Add more definitions here if necessary */

#endif  // __TS_INIT_H_
