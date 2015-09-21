/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab2a.h>


unsigned long int *esp;

void printprocstks(void) {
 
 int piterator;
 for(piterator = 0; piterator < NPROC; piterator++) {
 	
	if(piterator != 0 && isbadpid(piterator))
		continue;

	struct procent *proc = &proctab[piterator];
	
	kprintf("\nProcess [ %s ]",proc->prname);
	kprintf("\tPid: %d", piterator);
	kprintf("\nPrio: %d", proc->prprio);

 }
}

void test_lab1() {
	int pids[4];
	pids[0] = create(printloop, 2048, 25, "myproc1", 1, 11);
  	pids[1] = create(printloop, 2048, 25, "myproc2", 1, 22);
	pids[2] = create(printloop, 2048, 25, "myproc3", 1, 33);
  	pids[3] = create(printloop, 2048, 25, "myproc4", 1, 44);
  	
	//printprocstks();

	sleepms(100);

	putc(CONSOLE, 'P');
	resume(pids[0]);

	putc(CONSOLE, 'P');
	resume(pids[1]);

	putc(CONSOLE, 'P');
	resume(pids[2]);

	putc(CONSOLE, 'P');
	resume(pids[3]);


	sleepms(5000);
}

process	main(void)
{
	kprintf("\nLab2 - Part A, Problem 4 - Static Priority Scheduling of XINU Processes\n");
	test_lab1();
	return OK;
}
