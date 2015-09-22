/* cpuintensive.c - cpuintensive */

#include <xinu.h>
#include <lab2b.h>

#define LOOP1 5
#define LOOP2 1000000

void cpuintensive() {
  	
	/* LAB2BTODO */
	int i, j, k;
	int a[1000];

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

		kprintf("\nPID: %d \tLoop count: %d", pid, i);
	}

	kprintf("\nPID: %d, \tCPU-Time: %d", pid, proctab[pid].prcputime);

}
