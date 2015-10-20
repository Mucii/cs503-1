/* ready.c - ready */

#include <xinu.h>

// Extra one list is for non-Time-Sharing processes.
// That one is still ordered by priority.
qid16	readylists[TS_LEVELS+1];			/* Index of ready lists		*/
//qid16	readylist;					/* Index of ready list		*/

// rt readylist which holds the rt ready processes
// it adopts a round-robin policy between the processes
// assumption: resched handles rt processes separately and executes them at prio TS_LEVELS

qid16 rt_readylist;					/* Index of rt ready list 	*/

/**
 * @return the corresponding queue id of this priority.
 */
qid16 getQueueByPrio(pri16 prio) {
	int idx = prio;

	if (prio <= 0) {
		idx = 0;
	}
	else if (prio >= TS_LEVELS) {
		idx = TS_LEVELS;
	}

	return readylists[idx];
}


/**
 * If it's TS processes, insert it to the right place in the multi-level
 * feedback queue. If higher than TS priorities, add it to the rest readylist
 * that is still ordered by priority.
 */
void ts_insert(pid32 pid) {
	if (isbadpid(pid)) {
		return;
	}

	struct procent *prptr = &proctab[pid];
	if(prptr->prstate == PR_FREE) {
		return;
	}

	if (prptr->prprio >= TS_LEVELS) {
		// Not TS, that readylist is still ordered by priority, thus need insert().
		insert(pid, getQueueByPrio(prptr->prprio), prptr->prprio);
	}
	else {
		// TS (or NULLPROC), just find the right queue to enqueue().
		enqueue(pid, getQueueByPrio(prptr->prprio));
	}

	
	//kprintf("\nTS Insert [%s] pid [%d]", prptr->prname, pid);
}

/* ayush edit */

/* rt_insert
 * this method checks if it is a TS_PROC then use ts_insert as before
 * (I have assigned TS_PROC to both TS and higher system processes)
 * else insert into rt_readylist 
 */

void rt_insert(pid32 pid) {

	if(isbadpid(pid)) return;

	if(proctab[pid].prtype == TS_PROC) return ts_insert(pid);

	//kprintf("\nRT Insert [%s] pid [%d]", proctab[pid].prname, pid);
	
	// else insert into rt_readylist
	insert(pid, rt_readylist, proctab[pid].prprio);
}

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	// added for Lab2B
	//insert(pid, readylist, prptr->prprio);
	
	// added for LAB3
	rt_insert(pid);
	resched();

	return OK;
}
