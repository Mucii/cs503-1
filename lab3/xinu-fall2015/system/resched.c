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
 * ayush edit
 * the search is as follows:
 * 1. search for system processes in the readylists[TS_LEVELS]
 * 2. if not search for rt processes
 * 3. if not search for TS processes
 * 4. return the highest process found or SYSERR(if only NULLPROC is available
 */


local pid32 getHighestProc() {
	int i;

	// edited for LAB3
	// check for system process
	if(!isempty(readylists[TS_LEVELS]))
		return firstid(readylists[TS_LEVELS]);

	// check for rt process
	if(!isempty(rt_readylist))
		return firstid(rt_readylist);

	// check for ts process
	for (i = TS_LEVELS - 1; i >= 0; i--) {
		if (!isempty(readylists[i])) {
			/*
			 * Here it's using firstid(), rather than firstkey()!! Because in
			 * multi-level feedback queue, all are added by enqueue(), thus there is
			 * no "key" as that in old readylist.
			 */
			return firstid(readylists[i]);
		}
	}

	//kprintf("\nNo process Found!");
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
		//kprintf("\nHere!!");
		preempt = tstab[0].ts_quantum;
		return;
	}
		
	//kprintf("\nIn Resched highest pid  [%d] ", highest);
	
	// added for LAB3
	// priority for RT process for comparison with other processes is kept to be TS_LEVFL
	// within RT process it is as set in rt_create
	pri16 oldprio = ptold->prtype == TS_PROC ? ptold->prprio : TS_LEVELS;
	pri16 highestprio = proctab[highest].prtype  == TS_PROC ? proctab[highest].prprio : TS_LEVELS;

	// added for Lab2B
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		// because of timeout, or preempted

		// note rt process can never enter this condition hence priority never updated
		if (preempt < 1 && tsprio(oldprio)) {
			// set to tqexp only if it has used its entire quantum
			// ignore non TimeSharing processes
			ptold->prprio = tstab[oldprio].ts_tqexp;
		}
		

		// round-robin within RT processes
		// if both old and new processes are RT and old has higher priority
		// return with a new quantum
		if(ptold->prtype == RT_PROC && proctab[highest].prtype == RT_PROC && ptold->prprio > proctab[highest].prprio) {
			preempt = QUANTUM;
			//kprintf("\nScheduled [%s] time [%d] quanta [%d]", ptold->prname, myglobalclock, preempt);
			return;
			}
	
		// choose highest priority process
		// Note: only possibilities to come here
		// 1. type1 = TS_PROC , type2 = TS_PROC
		// 2. type1 = RT_PROC , type2 = TS_PROC
		
		if(oldprio > highestprio) {	
			// Don't change for non-TimeSharing processes
			preempt = tsprio(oldprio) ? tstab[oldprio].ts_quantum : QUANTUM;
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		
		// added for LAB3
		rt_insert(currpid);
		//ts_insert(currpid);
		//insert(currpid, readylist, ptold->prprio);
	}
	else {
		// because of blocked, currpid should have been added to some queue.

		if (tsprio(oldprio)) {
			ptold->prprio = tstab[oldprio].ts_slpret;
		}
	}


	/* Force context switch to highest priority ready process */
	if(proctab[highest].prtype == RT_PROC)
		currpid = dequeue(rt_readylist);
	else 
		currpid = dequeue(getQueueByPrio(proctab[highest].prprio));
	
	//currpid = dequeue(readylist);
	ptnew = &proctab[currpid];

	
	ptnew->prstate = PR_CURR;
	
	// added for LAB3
	pri16 newprio = ptnew->prtype == TS_PROC ? ptnew->prprio : TS_LEVELS;
	// edited for LAB2b
	preempt = tsprio(newprio) ? tstab[newprio].ts_quantum : QUANTUM;

	//kprintf("\nScheduled [%s] time [%d] quanta [%d]", ptnew->prname, myglobalclock, preempt);
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
