/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab4.h>

void receiver() {

	umsg32 msg;
//	sleepms(100);
	//recvclr();
	msg = receive();
	kprintf("\nPID %d msg read: %d", currpid, msg);

	msg = receive();
	kprintf("\nPID %d msg read: %d", currpid, msg);
	
	msg = receive();
	kprintf("\nPID %d msg read: %d", currpid, msg);

	msg = receive();
	kprintf("\nPID %d msg read: %d", currpid, msg);

}

void sender(pid32 pid, int maxtime, umsg32 msg) {
	status st = sendb(pid, msg, maxtime);
	if(st == TIMEOUT) {
		kprintf("\nPID %d Send time out", getpid());
	}
}

void test_sendb() {

	pid32 pid;
	resched_cntl(DEFER_START);
	pid = create(receiver, 2048, 20, "Proc1", 0, NULL);

	resume( pid);	
	resume( create(sender, 2048, 20, "Proc2", 3, pid, 100, 2));
	resume( create(sender, 2048, 20, "Proc3", 3, pid, 100, 3));
	resume( create(sender, 2048, 20, "Proc4", 3, pid, 0, 4));
	resume( create(sender, 2048, 20, "Proc5", 3, pid, 100, 5));
	resched_cntl(DEFER_STOP);
}

process	main(void)
{
	/* Lab4 */
	test_sendb();	
	return OK;
}
