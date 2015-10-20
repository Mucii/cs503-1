/* signal.c - signal */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  signal  -  Signal a semaphore, releasing a process if one is waiting
 *------------------------------------------------------------------------
 */
syscall	signal(
	  sid32		sem		/* ID of semaphore to signal	*/
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	semptr= &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}

	/* ayush edit 
	 * added updates for deadlock detection data strcutres
	 */

	// reduce allocation
	if(allocation[getpid()][sem] > 0) 
		allocation[getpid()][sem]--;

	if ((semptr->scount++) < 0) {	/* Release a waiting process */
		
		// reduce request
		if(request[firstid(semptr->squeue)][sem] > 0)
			request[firstid(semptr->squeue)][sem]--;
		ready(dequeue(semptr->squeue));
	} else {
		available[sem]++;
	}
	restore(mask);
	return OK;
}
