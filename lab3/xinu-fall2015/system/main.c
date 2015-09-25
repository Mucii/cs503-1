/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab2b.h>

void test_allcpu() {

	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc4", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc5", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc6", 0, NULL));
}

void test_allio() {
	uint32 sleeptime = 20;
	resume( create(iointensive, 2048, 20, "IOProc1", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc2", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc3", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc4", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc5", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc6", 1, sleeptime));
}


void test_half1() {

	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));
	
	uint32 sleeptime = 10;
	resume( create(iointensive, 2048, 20, "IOProc1", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc2", 1, sleeptime));
	resume( create(iointensive, 2048, 20, "IOProc3", 1, sleeptime));

} 

void test_half2() {
	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));
	
	uint32 sleeptime = 5;
	resume( create(iointensive, 2048, 20, "IOProc1", 1, 7));
	resume( create(iointensive, 2048, 20, "IOProc2", 1, 13));
	resume( create(iointensive, 2048, 20, "IOProc3", 1, 29));


}

void test_starvation() {

}

process	main(void)
{
  	/* Lab2BTODO */
  	kprintf("===== Lab2B =====\n");
	
	
	kprintf("===== Lab2B - Problem 3.1 =====\n");
  	// All CPU intensive
	// test_allcpu();

  	kprintf("===== Lab2B - Problem 3.2 =====\n");
  	// All IO intensive
	// test_allio();

  	kprintf("===== Lab2B - Problem 3.3 =====\n");
  	// Half CPU-, half IO- intensive, v1
	//test_half1();

  	kprintf("===== Lab2B - Problem 3.4 =====\n");
  	// Half CPU-, half IO- intensive, 
	test_half2();

  	kprintf("===== Lab2B - Bonus Problem =====\n");
  	// One CPU-intensive, one hybrid

	kprintf("\nPID: %d, CPU-Time: %d", getpid(), proctab[getpid()].prcputime);
	return OK;
}
