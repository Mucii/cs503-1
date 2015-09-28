/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/


/* ayush edit 
 * array of readylist for each level from 0 - NUMLEVEL - 1
 * level NUMLEVEL is for higher priority processes to which TS scheduling is not applicable */
qid16	multiqueue[NUMLEVELS + 1];	/* Indices of readylist for each level */

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
	
	/* TS scheduler policy
	 * add to level NUMLEVELS if it is higher priority system process
	 * else to level as indicated by the current priority
	 */

	if(prptr->prprio < NUMLEVELS) {
		insert(pid, multiqueue[prptr->prprio], prptr->prprio);
	} else { 
		insert(pid, multiqueue[NUMLEVELS], prptr->prprio);
	}

	/* since process is just added to readyqueue set waittime = 0 */
	prptr->waittime = 0;
	resched();

	return OK;
}
