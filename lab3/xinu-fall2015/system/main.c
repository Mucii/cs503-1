/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab2b.h>

process	main(void)
{
  	/* Lab2BTODO */
  	kprintf("===== Lab2B =====\n");
  	
	/*
	kprintf("\nDispatcher Table:\n");
	int i = 0;
	for(; i < NUMLEVELS; i++) kprintf("\n#%d\t%d\t%d\t%d", i, tstab[i].ts_quantum, tstab[i].ts_tqexp, tstab[i].ts_slpret);
	
	*/
	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));


	uint32 sleeptime = 20;
	resume( create(iointensive, 2048, 20, "IOProc1", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc2", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc3", 1, sleeptime));

	kprintf("===== Lab2B - Problem 3.1 =====\n");
  	// All CPU intensive

  	kprintf("===== Lab2B - Problem 3.2 =====\n");
  	// All IO intensive

  	kprintf("===== Lab2B - Problem 3.3 =====\n");
  	// Half CPU-, half IO- intensive, v1

  	kprintf("===== Lab2B - Problem 3.4 =====\n");
  	// Half CPU-, half IO- intensive, 

  	kprintf("===== Lab2B - Bonus Problem =====\n");
  	// One CPU-intensive, one hybrid

	kprintf("\nPID: %d, \tCPU-Time: %d", getpid(), proctab[getpid()].prcputime);
	return OK;
}
