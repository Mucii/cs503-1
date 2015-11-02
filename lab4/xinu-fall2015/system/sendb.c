/* sendb.c - sendb */

#include <xinu.h>
#include <lab4.h>

/*---------------------------------------------------------------------------
 *  sendb  -  Pass a message to a process and start recipient if waiting, but
 *  may block a while when message is not able to deliver.
 *---------------------------------------------------------------------------
 */
syscall	sendb(
	pid32		pid,		/* ID of recipient process	*/
	umsg32	msg,		/* Contents of message		*/
	int32		maxwait	/* How long a process may block before it returns */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr, *ptr;	/* Ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	ptr = &proctab[currpid];
	
	if (prptr->prstate == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (prptr->prhasmsg) {

		// invalid wait time
		if(maxwait < 0) {
			restore(mask);
			return SYSERR;
		}

		// make process sleep if timeout is finite 
		// if maxwait = 0 -> not inserted into sleepq,
		// it can only be woken up by the receiving process */
		if(maxwait > 0) {
			if(insert(currpid, sleepq, maxwait) == SYSERR) {
				restore(mask);
				return SYSERR;
			}
			prptr->prstate = PR_SND;
		} else {
			prptr->prstate = PR_SNDI;
		}
		
		kprintf("\nPID: %d, Send Block.", currpid);
		s_enqueue(currpid, sendlists[pid]);
		ptr->prsndmsg = msg;
		ptr->prsndflag = TRUE;
		resched();
	}
	// Note: timeout can happen only for PR_SND
	
	status ret = OK;
	if (ptr->prsndflag) {
		//kprintf("\nPID %d flag set", currpid);	
		ptr->prsndflag = FALSE;
		ret = TIMEOUT;
		prptr->prmsg = TIMEOUT;
	} else {
		prptr->prmsg = msg;		/* Deliver message		*/
		kprintf("\nPID: %d, Send Complete", currpid);	
	}
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}

	restore(mask);		/* Restore interrupts */
	return ret;
}
