/* ts_init.h - CS503 15Fall, Lab 2 - Part B */

#ifndef __TS_INIT_H_
#define __TS_INIT_H_

#include <lab2b.h>

struct ts_ent {
  int ts_tqexp;  	// new priority: CPU-intensive (time quantum expired)
  int ts_slpret;  	// new priority: I/O-intensive (sleep return)
  int ts_quantum;  	// new time slice

  /* added variables for bonus problem */
  int ts_maxwait;	// max time since a full time slice
  int ts_lwait;		// level to be raised if ts_maxwait is expired
};
extern struct ts_ent tstab[];

/*  ayush edit
 *  added NUMLEVELS for the multilevel queue 
 *  total levels = NUMLEVELS + 1 (for higher priority system processes)*/
#define NUMLEVELS 60
#endif  // __TS_INIT_H_
