/* iointensive.c - iointensive */

#include <xinu.h>
#include <lab2b.h>

#define LOOP1 5
#define LOOP2 10

void iointensive(uint32 time) {
  	/* LAB2BTODO */

	/* ayush edit */
	int i, j;

	pid32 pid = getpid();
	uint32 sleeptime;
	for(i = 0; i <= LOOP1; i++) {
		for(j = 0; j <= LOOP2; j++) {
		
			sleeptime = myglobalclock;
			sleepms(time);
			// use below printf to see the if the desired sleep time is achieved 
			// kprintf("\nPID %d, Sleep Diff %d", pid, (myglobalclock - sleeptime - time));
		}
		kprintf("\nPID: %d \tLoop count: %d", pid, i);
	}

	kprintf("\nPID: %d, CPU-Time: %d", pid, proctab[pid].prcputime);

}
