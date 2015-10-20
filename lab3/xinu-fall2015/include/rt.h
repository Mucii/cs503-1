/* rt.h - CS503 15Fall, Lab 3 */

#ifndef __RT_H_
#define __RT_H_

pid32	rt_create(int rt_period, int rt_comp, void *funcaddr, uint32 ssize, char *name, uint32 nargs, ...);

/* ayush edit */

// RMS slack factor
extern float rmsslk;

// data structures for deadlock detection
extern uint32 available[NSEM];
extern uint32 request[NPROC][NSEM];
extern uint32 allocation[NPROC][NSEM];

// modified insert method to handle rt scheduling
void rt_insert(pid32 pid);

#endif  // __RT_H_
