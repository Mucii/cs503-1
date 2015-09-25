/* ready.c - ready */

#include <xinu.h>

qid16	readylist;			/* Index of ready list		*/


/* AYUSH EDIT */
qid16	ioreadylist;			/* Index of io-bound ready lisy */
qid16	cpureadylist;			/* Index of cpu-ready list	*/
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
	//insert(pid, readylist, prptr->prprio);
	
	
	if(prptr->prprio < NUMLEVELS) {
		insert(pid, multiqueue[prptr->prprio], prptr->prprio);
		// kprintf("READY: Inserted into cpureadylist\n");
	} else { 
		insert(pid, multiqueue[NUMLEVELS], prptr->prprio);
		// kprintf("READY: Inserted into ioreadylist %d\n", prptr->prprio);
	}

	prptr->waittime = 0;
	resched();

	return OK;
}
