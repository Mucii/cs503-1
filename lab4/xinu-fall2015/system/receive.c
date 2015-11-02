/* receive.c - receive */

#include <xinu.h>

void unblock() {
	pid32 pid;
	struct procent *sendptr;
	pid = s_dequeue(sendlists[currpid]);
	if(pid != EMPTY && pid != SYSERR) {
		sendptr = &proctab[pid];
		sendptr->prsndflag = FALSE;
		kprintf("\nPID: %d unblocked PID %d ", currpid, pid);
		if(sendptr->prstate == PR_SND) {
			unsleep(pid);
			ready(pid);
		} else if (sendptr->prstate == PR_SNDI) {
			ready(pid);
		}
	}
}

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	unblock();			/* Unblock if any process waiting to send */
	restore(mask);
	return msg;
}
