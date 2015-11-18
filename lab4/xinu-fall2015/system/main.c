/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab4.h>

int myrecvhandler(umsg32 msg) {
        kprintf("\nTime %d, PID: %d Async msg received = %d buf %d\n", 
		myglobalclock, currpid, msg, *(proctab[currpid].userbuf));
	return OK;
}

int myalarmhandler() {
	kprintf("\nTime %d PID: %d ALARM!!!!", myglobalclock, currpid);
	return OK;
}

int recvsighandler() {

        umsg32 msg = receive();
	kprintf("\nTime %d PID: %d Signal msg received = %d\n", myglobalclock, currpid, msg);
	return OK;
}

void rec3() {
	
	if(registercbsig(ARECV, recvsighandler, 0 ) != OK) {

		kprintf("\nRecv Signal Handler Registration failed.");
		return;
	}
	
	if(registercbsig(AALRM, myalarmhandler, 1000 ) != OK) {

		kprintf("\nRecv Signal Handler Registration failed.");
		return;
	}
	// while away cycles till you get a msg
	while(1) {
	}
}

void rec2() {
	
	umsg32 buf = 10;
	if(registercb(&buf, myrecvhandler ) != OK) {

		kprintf("\nRecv Handler Registration failed.");
		return;
	}

	if(registercbsig(AALRM, myalarmhandler, 10000 ) != OK) {

		kprintf("\nRecv Signal Handler Registration failed.");
		return;
	}
	// while away cycles till you get a msg
	while(1) {
			
	}
}

void receiver() {

	int numsender = 8;
	int sleeptime = 50;
	umsg32 msg[numsender];
	int i = 0;
	sleepms(sleeptime);
	for(; i < numsender; i++) {
		msg[i] = receive();
		kprintf("\nTime %d, PID: %d msg read: %d", myglobalclock, currpid, msg[i]);

	}
}

void sender(pid32 pid, int maxtime, umsg32 msg) {
	
//	sleepms(currpid * 100);

	status st = sendb(pid, msg, maxtime);
	if(st == TIMEOUT) {
		kprintf("\nTime %d, PID %d Send time out", myglobalclock, getpid());
	}
}

void test_sendb() {

	pid32 pid;

	pid = create(receiver, 2048, 20, "Proc1", 0, NULL);
	resched_cntl(DEFER_START);
	resume( pid);	
	resume( create(sender, 2048, 20, "Proc2", 3, pid, 100, 200));
	resume( create(sender, 2048, 20, "Proc3", 3, pid, 0, 300));
	resume( create(sender, 2048, 20, "Proc4", 3, pid, 10, 400));
	resume( create(sender, 2048, 20, "Proc5", 3, pid, 60, 500));
	resume( create(sender, 2048, 20, "Proc6", 3, pid, 50, 600));
	resume( create(sender, 2048, 20, "Proc7", 3, pid, 20, 700));
	resume( create(sender, 2048, 20, "Proc8", 3, pid, 150, 800));
	resume( create(sender, 2048, 20, "Proc9", 3, pid, 0, 900));
	
	resched_cntl(DEFER_STOP);
}

void test_async() {

	pid32 pid;

	pid = create(rec2, 2048, 20, "Rec1", 0, NULL);
	resched_cntl(DEFER_START);
	resume( pid);	
	resume( create(sender, 2048, 20, "Proc2", 3, pid, 100, 200));
	resume( create(sender, 2048, 20, "Proc3", 3, pid, 100, 300));
	resume( create(sender, 2048, 20, "Proc4", 3, pid, 0, 400));
	resume( create(sender, 2048, 20, "Proc5", 3, pid, 100, 500));
	resume( create(sender, 2048, 20, "Proc6", 3, pid, 100, 600));
	resume( create(sender, 2048, 20, "Proc7", 3, pid, 100, 700));
	resume( create(sender, 2048, 20, "Proc8", 3, pid, 0, 800));
	resume( create(sender, 2048, 20, "Proc9", 3, pid, 100, 900));
	resched_cntl(DEFER_STOP);
}

void test_bonus() {

	pid32 pid;

	pid = create(rec3, 2048, 20, "Rec1", 0, NULL);
	resched_cntl(DEFER_START);
	resume( pid);	
	resume( create(sender, 2048, 20, "Proc2", 3, pid, 100, 200));
	resume( create(sender, 2048, 20, "Proc3", 3, pid, 100, 300));
	resume( create(sender, 2048, 20, "Proc4", 3, pid, 0, 400));
	resume( create(sender, 2048, 20, "Proc5", 3, pid, 100, 500));
	resume( create(sender, 2048, 20, "Proc6", 3, pid, 100, 600));
	resume( create(sender, 2048, 20, "Proc7", 3, pid, 100, 700));
	resume( create(sender, 2048, 20, "Proc8", 3, pid, 0, 800));
	resume( create(sender, 2048, 20, "Proc9", 3, pid, 100, 900));
	resched_cntl(DEFER_STOP);
}

process	main(void)
{
	/* Lab4 */
	test_sendb();
	//test_async();
	//test_bonus();
	return OK;
}
