/* cpuintensive.c - cpuintensive */

#include <xinu.h>
#include <lab2b.h>

#define LOOP1 5
#define LOOP2 1000000

void cpuintensive() {
  	
	/* LAB2BTODO */
	int i, j, k;
	int a[1000];
	uint32 inittime = myglobalclock;
	for(i = 0; i < 1000; i++) a[i] = i;
	pid32 pid = getpid();

	for(i = 0; i <= LOOP1; i++) {
		for(j = 0; j <= LOOP2; j++) {
			int tmp[1000];
			for(k = 0; k < 100; k++) {
				a[k] = (a[k] + 1) % 1000;
				tmp[k] = a[k];
			}
		}
		/* the waitime till now can be given by myglobalclock - starttime of the process */
		kprintf("\nPID: %d \tLoop count: %d Wait-Time %d", pid, i, myglobalclock - inittime);
	}

	kprintf("\nPID: %d, CPU-Time: %d Wait-Time %d", pid, proctab[pid].prcputime, myglobalclock - inittime);

}
