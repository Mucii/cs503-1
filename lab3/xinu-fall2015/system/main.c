/*  main.c  - main */

#include <xinu.h>
#include <lab3.h>
void test_CPU() {

	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	//resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));
}

void test_RT() {

	resume( rt_create(50, 10, myperiodicrtapp, 2048, "RTProc1", 2, 50, 10));
	resume( rt_create(200, 40, myperiodicrtapp, 2048, "RTProc2", 2, 200, 40));
	resume( rt_create(100, 15, myperiodicrtapp, 2048, "RTProc3", 2, 100, 15));
	resume( rt_create(100, 60, myperiodicrtapp, 2048, "RTProc4", 2, 100, 15));
	

}

process	main(void)
{
	/* Lab3TODO */
	kprintf("===== Lab3 =====\n");
	test_CPU();
	test_RT();
	//test_allcpu();
	return OK;
}
