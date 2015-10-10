/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/**
 * @return 1 (true) if @param prio is a time sharing process' priority.
 */
bool8 tsprio(pri16 prio) {
  return prio > 0 && prio < TS_LEVELS;
}


/**
 * @return the highest priority process id in readylists.
 * To convert to qid16, call getQueueByPrio().
 */
local pid32 getHighestProc() {
	int i;
	for (i = TS_LEVELS; i >= 0; i--) {
		if (nonempty(readylists[i])) {
			/*
			 * Here it's using firstid(), rather than firstkey()!! Because in
			 * multi-level feedback queue, all are added by enqueue(), thus there is
			 * no "key" as that in old readylist.
			 */
			return firstid(readylists[i]);
		}
	}

	// It's possible to reach here, i.e. when only NULLPROC is there, executing.
	return SYSERR;
}


/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	pid32 highest;
	if ((highest = getHighestProc()) == SYSERR) {
		// no other processes, only NULLPROC, thus no need to reschedule
		preempt = tstab[0].ts_quantum;
		return;
	}

	// added for Lab2B
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		// because of timeout, or preempted

		if (preempt < 1 && tsprio(ptold->prprio)) {
			// set to tqexp only if it has used its entire quantum
			// ignore non TimeSharing processes
			ptold->prprio = tstab[ptold->prprio].ts_tqexp;
		}

		//if (ptold->prprio > firstkey(readylist)) {
		if (ptold->prprio > proctab[highest].prprio) {
			// Don't change for non-TimeSharing processes
			preempt = tsprio(ptold->prprio) ? tstab[ptold->prprio].ts_quantum : QUANTUM;
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		ts_insert(currpid);
		//insert(currpid, readylist, ptold->prprio);
	}
	else {
		// because of blocked, currpid should have been added to some queue.

		if (tsprio(ptold->prprio)) {
			ptold->prprio = tstab[ptold->prprio].ts_slpret;
		}
	}

	/* Unnecessary starvation code is not included in the code base for Lab3. */

	/* Force context switch to highest priority ready process */

	currpid = dequeue(getQueueByPrio(proctab[highest].prprio));
	//currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	// added for Lab2B
	preempt = tsprio(ptnew->prprio) ? tstab[ptnew->prprio].ts_quantum : QUANTUM;
	//preempt = QUANTUM;		/* Reset time slice for process	*/

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
