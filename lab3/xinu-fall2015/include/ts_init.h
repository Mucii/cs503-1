/* ts_init.h - CS503 15Fall, Lab 2 - Part B */

#ifndef __TS_INIT_H_
#define __TS_INIT_H_

// added for Lab2B
struct ts_ent {
	int ts_tqexp;    // new priority: CPU-intensive (time quantum expired)
	int ts_slpret;   // new priority: I/O-intensive (sleep return)
	int ts_quantum;  // new time slice

	// Unnecessary code of starvation problem isn't included in the base for Lab3.
};
extern struct ts_ent tstab[];

#ifndef TS_LEVELS
#define TS_LEVELS 60
#endif


// added for Lab2B
extern volatile uint32 myglobalclock;

// Utility functions for my multi-feedback queue implementation.
bool8 tsprio(pri16 prio);
qid16 getQueueByPrio(pri16 prio);
void ts_insert(pid32 pid);

#endif  // __TS_INIT_H_
