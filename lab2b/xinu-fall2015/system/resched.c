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

	/* TS scheduler policy
	 * 1. check ptold.prstate 
	 * 2. if current process still has highest priority resume without rescheduling
	 *    	insert into the readylist (see clkhandler.c: handles priority update on time slice expiry)
	 *    else if sleeping process update prioirty according to dispatch table
	 * 4. pick highest prioirty process from the readylist
	 * */

	// kprintf("\nCLK %d Process [%s] \tPrio: %d \tcputime: %d\n", ctr1000, ptold->prname, ptold->prprio, ptold->prcputime);
	
	uint32 oldprio = ptold->prprio;
	
	int level = NUMLEVELS;
	/* find the highest level which is not empty */
	while(isempty(multiqueue[level]) && level > 0) level--;


	if(ptold->prstate == PR_CURR) {

		// if current process has highest priority resume
		if(ptold->prprio > firstkey(multiqueue[level])) {
			return;
		}
		

		// else push into readylist
		// Note: if it a process has priority higher than NUMLEVELS - 1
		// it is considered to be a system process and added to level NUMLEVEL
		// else a process is added to the appropriate level according to current priority
		
		ptold->prstate = PR_READY;
		if(ptold->prprio < NUMLEVELS) {
			insert(currpid, multiqueue[ptold->prprio], ptold->prprio);
		} else { 
			insert(currpid, multiqueue[NUMLEVELS], ptold->prprio);
		}


	} else if(ptold->prstate == PR_SLEEP) {
		
		// update priority
		if(ptold->prprio < NUMLEVELS)
			ptold->prprio = tstab[oldprio].ts_slpret;
	
	}

	
	currpid = dequeue(multiqueue[level]);

	// Apply check for nullprocess being dequeued and readlist is not empty
	//  if yes insert back the null process into the readylist at the tail
	
	if(currpid == NULLPROC && !isempty(multiqueue[level]) ) {
		insert(currpid, multiqueue[level], NULLPROC);
		currpid = dequeue(multiqueue[level]);
	}


	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
  	
	/* set preempt value to the quantum defined for current priority */
	preempt = tstab[ptnew->prprio].ts_quantum;		/* Reset time slice for process	*/
	
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
