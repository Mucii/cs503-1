/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
  /* LAB2BTODO: Modify this function to implement TS scheduling */

	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	/* AYUSH EDIT lab2b */
	
	/* TS scheduler policy
	 * 1. check ptold.prstate
	 * 2. update the priority according to the TS dispatcher based on desired process state
	 * 3. if current process still has highest priority resume with new quantum
	 * 4. insert into the readlist with updated priority
	 * 5. pick highest prioirty process from the readylist
	 * */

	// kprintf("\nCLK %d Process [%s] \tPrio: %d \tcputime: %d\n", ctr1000, ptold->prname, ptold->prprio, ptold->prcputime);
	
	uint32 oldprio = ptold->prprio;

	if(ptold->prstate == PR_CURR) {

		// update priority
		ptold->prprio = tstab[oldprio].ts_tqexp;
		
		// if current process has highest priority resume with new quantum
		if((ptold->prprio > firstkey(ioreadylist) && ! isempty(ioreadylist)) || (ptold->prprio > firstkey(cpureadylist) && isempty(ioreadylist))) {
			preempt = tstab[ptold->prprio].ts_quantum;
			return;
		}

		// push into readlist
		ptold->prstate = PR_READY;
		//insert(currpid, readylist, ptold->prprio);
		if(ptold->prprio < 50) {
			insert(currpid, cpureadylist, ptold->prprio);
			// kprintf("Inserted into cpureadylist\n");
		} else {
			// kprintf("Inserted into ioreadylist\n");
			insert(currpid, ioreadylist, ptold->prprio);
		}

	} else if(ptold->prstate == PR_SLEEP) {
		
		// update priority
		ptold->prprio = tstab[oldprio].ts_slpret;
	
	}


	/* Force context switch to highest priority ready process */
	if(!isempty(ioreadylist))
		currpid = dequeue(ioreadylist);
	else
		currpid = dequeue(cpureadylist);
	/* Apply check for nullprocess being dequeued and readlist is not empty
	 * if yes insert back the null process into the readylist at the tail */
	if(currpid == NULLPROC && !isempty(cpureadylist) ) {
		insert(currpid, cpureadylist, NULLPROC);
		currpid = dequeue(cpureadylist);
	}


	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
  	
	/* AYUSH EDIT lab2b */
	preempt = tstab[ptnew->prprio].ts_quantum;		/* Reset time slice for process	*/
	
	// kprintf("\nProcess [%s] \tPrio: %d \tcputime: %d ", ptold->prname, ptold->prprio, ptold->prcputime);
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
