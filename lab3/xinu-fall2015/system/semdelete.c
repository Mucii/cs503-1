/* semdelete.c - semdelete */

#include <xinu.h>

/*------------------------------------------------------------------------
 * semdelete  -  Delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
syscall	semdelete(
	  sid32		sem		/* ID of semaphore to delete	*/
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to semaphore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	
	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	semptr->sstate = S_FREE;
	
	/* ayush edit
	 * update deadlock detection data structures */

	// make resource availability zero
	available[sem] = 0;
	resched_cntl(DEFER_START);
	while (semptr->scount++ < 0) {	/* Free all waiting processes	*/
		//update request count for the processes waiting
		pid32 pid = getfirst(semptr->squeue);
		request[pid][sem] = 0;
		ready(pid);
	}
	resched_cntl(DEFER_STOP);
	restore(mask);
	return OK;
}
