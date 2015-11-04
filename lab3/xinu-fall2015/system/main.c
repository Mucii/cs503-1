/*  main.c  - main */

#include <xinu.h>
#include <lab3.h>
void test_CPU() {

	resched_cntl(DEFER_START);
	resume( create(cpuintensive, 2048, 20, "CPUProc1", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc2", 0, NULL));
	resume( create(cpuintensive, 2048, 20, "CPUProc3", 0, NULL));
	resched_cntl(DEFER_STOP);
}


void test_RT() {

	resched_cntl(DEFER_START);
	//resume( rt_create(200, 15, myperiodicrtapp, 2048, "RTProc2", 2, 100, 20));
	//resume( rt_create(500, 10, myperiodicrtapp, 2048, "RTProc1", 2, 50, 10));
	//resume( rt_create(300, 10, myperiodicrtapp, 2048, "RTProc3", 2, 150, 10));
	resume( rt_create(100, 20, myperiodicrtapp, 2048, "RTProc4", 2, 20, 8));
	resume( rt_create(50, 10, myperiodicrtapp, 2048, "RTProc4", 2, 20, 8));
	resched_cntl(DEFER_STOP);

}

void mytestapp(char c, sid32 sem1, sid32 sem2) {

	waitd(sem1);
	kputc(c);	
	sleepms(100);
	waitd(sem2);
	kputc(c);
	signal(sem2);
	signal(sem1);

}

void mytestapp2(char c, sid32 sem1) {

	waitd(sem1);
	kputc(c);
	sleepms(200);
	signal(sem1);
}

void test_deadlock() {
	sid32 sem1, sem2, sem3;

	if((sem1 = semcreate(1)) == SYSERR)
		return;
	if((sem2 = semcreate(1)) == SYSERR)
		return;
	if((sem3 = semcreate(1)) == SYSERR)
		return;

	kprintf("\nStarting Processes\n");

	resched_cntl(DEFER_START);
	resume( create(mytestapp, 2048, 20, "SemProc1", 3, 'C', sem1, sem2));
	resume( create(mytestapp2, 2048, 20,"SemProc2", 2, 'B', sem1));
	resume( create(mytestapp, 2048, 20, "SemProc3", 3, 'A', sem2, sem3));
	resume( create(mytestapp, 2048, 20, "SemProc4", 3, 'D', sem3, sem1));
	resched_cntl(DEFER_STOP);

//	resume( create(mytestapp2, 2048, 20, "SemProc2", 3, 'B', sem1));
	
}

process	main(void)
{
	/* Lab3TODO */
	kprintf("===== Lab3 =====\n");
	//test_CPU();
	//test_RT();
	test_deadlock();
	return OK;
}
