/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <lab4.h>

void receiver() {

	umsg32 msg;
//	sleepms(100);
	recvclr();
	msg = receive();
	kprintf("\nPID: %d msg read: %d", currpid, msg);

	msg = receive();
	kprintf("\nPID: %d msg read: %d", currpid, msg);
	
	msg = receive();
	kprintf("\nPID: %d msg read: %d", currpid, msg);

	msg = receive();
	kprintf("\nPID: %d msg read: %d", currpid, msg);

}

void sender(pid32 pid, int maxtime, umsg32 msg) {
	
	sleepms(currpid * 1000);

	status st = sendb(pid, msg, maxtime);
	if(st == TIMEOUT) {
		kprintf("\nPID %d Send time out", getpid());
	}
}

int myrecvhandler(umsg32 msg) {
        kprintf("\nPID: %d msg received = %d\n", currpid, msg);
	return OK;
}

void rec2() {
	
	umsg32 buf = 1, lastmsg = - 1;
	if(registercb(&buf, myrecvhandler ) != OK) {

		kprintf("\nRecv Handler Registration failed.");
		return;
	}

	// while away cycles till you get a msg
	while(1) {
		if(lastmsg != buf) {
			kprintf("\nPID: %d buf updated= %d", currpid, buf);
			lastmsg = buf;
		}
	}
}
void test_sendb() {

	pid32 pid;

	pid = create(receiver, 2048, 20, "Proc1", 0, NULL);
	resched_cntl(DEFER_START);
	resume( pid);	
	resume( create(sender, 2048, 20, "Proc2", 3, pid, 100, 200));
	resume( create(sender, 2048, 20, "Proc3", 3, pid, 100, 300));
	resume( create(sender, 2048, 20, "Proc4", 3, pid, 0, 400));
	resume( create(sender, 2048, 20, "Proc5", 3, pid, 100, 500));
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
	resched_cntl(DEFER_STOP);
}
process	main(void)
{
	/* Lab4 */
	//test_sendb();
	test_async();
	return OK;
}
