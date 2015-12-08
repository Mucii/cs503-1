/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>


void test_bs() {
	int i;
	bsd_t bsid = allocate_bs(10);

	kprintf("\nPID %d BS allocated %d", currpid, bsid);

	int src[NBPG / 4];
	int dst[NBPG / 4];

	for (i = 0; i < NBPG / 4; i++) {
		src[i] = 10;
		dst[i] = 0;
	}
	
	src[0] = src[1] = 5;

	write_bs((char *) &src, bsid, 0);
	read_bs((char *) &dst, bsid, 0);
	
	for(i = 0; i < 10; i++)
		kprintf("\nPID %d dst[i] %d", currpid, dst[i]);
	
}


void proc1() {

	int i;
	kprintf("\nPID %s Hey wastup!!", proctab[currpid].prname);
	char *buf = vgetmem(25*NBPG);
	for(i = 0; i < 25*NBPG; i++)
		buf[i] = 'a';
	char *duf = vgetmem(40 * NBPG);
	for(i = 0; i < 40*NBPG; i++)
		duf[i] = 'b';
	
	vfreemem(buf, 25*NBPG);
	vfreemem(duf, 40*NBPG);
	
	kprintf("\nPID %d Numfaults : %d", currpid, get_faults());
	
}

void proc2() {

	int i;
	kprintf("\nPID %s Hey I am fine wat abt u!!", proctab[currpid].prname);

	int size = 10;
	char out1[size], out2[size];

	char *duf = vgetmem(size * NBPG);
	for(i = 0; i < size*NBPG; i+= NBPG) {
		duf[i] = 'b';
		out1[i / NBPG] = duf[i];


	}


	for(i = 0; i < size*NBPG; i+= NBPG) {
		duf[i] = 'z';
		out2[i / NBPG] = duf[i];

	}

	vfreemem(duf, size * NBPG);
	kprintf("\n\n===========================================\n");
	for(i = 0; i < size; i++)
		kprintf("%c ", out1[i]);
	
	kprintf("\n=============================================\n");	

	kprintf("\n\n===========================================\n");
	for(i = 0; i < size; i++)
		kprintf("%c ", out2[i]);
	
	kprintf("\n=============================================\n");	

	kprintf("\n================PID %d Numfaults : %d===============", currpid, get_faults());
}

void test_without_virtual() {

	resume (create(proc1, INITSTK, INITPRIO, "Proc1", 0,  NULL));
	resume (create(proc2, INITSTK, INITPRIO, "Proc2", 0,  NULL));
}

void test_virtual() {

	//resume (vcreate(proc1, INITSTK, 650, INITPRIO, "VProc1", 0,  NULL));
	
	//sleepms(50);
	//resume (vcreate(proc1, INITSTK, 200, INITPRIO, "VProc2", 0,  NULL));

	//sleepms(50);
	resume (vcreate(proc2, INITSTK, 500, INITPRIO, "VProc3", 0,  NULL));

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

	//test_without_virtual();

	test_virtual();
	//test_bs();
	return OK;
}




