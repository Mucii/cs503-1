/* cpuintensive.c - cpuintensive */

#include <xinu.h>
#include <lab2b.h>

void cpuintensive() {
	// added in Lab2-B

	int starting = myglobalclock;

	// added for Lab2B
	int i, j;
	int LOOP1 = 10;
	int LOOP2 = 10000000;

	struct procent *pr = &proctab[currpid];

	int v = 0;
	for (i=0; i<LOOP1; i++) {
		for (j=0; j<LOOP2; j++) {
			// Insert code that performs memory copy (slow) and/or
			// ALU operations (fast).
			// Note: this loop consumes significant CPU cycles.
			v += i * j;
		}

		// Using kprintf print the pid followed the outer loop count i,
		// the process's priority and remaining time slice (preempt).
		kprintf("PID: %d, Loop: %d, Priority: %d, Remaining Time Slice: %d\n", currpid, i, pr->prprio, preempt);
	}

	// Print the CPU time consumed by the process that is recorded in the
	// prcputime field of the current process's process table entry.

	int ending = myglobalclock;

	// Printing wall-clock is only for debugging
	kprintf("===== PID %d, CPU TIME: %d, Wall Clock: %d\n", currpid, pr->prcputime, ending - starting);
}
