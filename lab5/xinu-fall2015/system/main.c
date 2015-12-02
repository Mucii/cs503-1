/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void proc1() {

	kprintf("\nPID %s Hey wastup!!", proctab[currpid].prname);
}

void proc2() {

	kprintf("\nPID %s Hey I am fine wat abt u!!", proctab[currpid].prname);
}

void test_without_virtual() {

	resume (create(proc1, INITSTK, INITPRIO, "Proc1", 0,  NULL));
	resume (create(proc2, INITSTK, INITPRIO, "Proc2", 0,  NULL));
}

void test_virtual() {

	resume (vcreate(proc1, INITSTK, 100, INITPRIO, "VProc1", 0,  NULL));

}
process	main(void)
{
	/* Start the network */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	netstart();

	/* Initialize the page server */
	/* DO NOT REMOVE OR COMMENT THIS CALL */
	psinit();

	kprintf("=== Lab5 ===\n");

	test_without_virtual();

	test_virtual();
	return OK;
}




