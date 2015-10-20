/* waitd.c - waitd */

#include <xinu.h>
#include <lab3.h>


/*------------------------------------------------------------------------
 *  waitd  -  Cause current process to wait on a semaphore.
 *  If a deadlock is resulted, waitd() behaves as a non-blocking call,
 *  with an error status that indicates a DEADLOCK failure.
 *  @return SYSERR if possible deadlock else OK
 *------------------------------------------------------------------------
 */
syscall	waitd(
	sid32		sem		/* semaphore on which to wait  */
	)
{
	/* ayush edit */
	
	/* the deadlock detection algorithm is based on resource allocation graph as follows
	 * 1. temporaily update the request for the current process on the semaphore requested
	 * 2. find processes which are holding at least one resource and mark them not finished
	 * 3. do
	 * 4. 	find a process which can finish its requests with current allocation+available
	 * 5.	if found: mark that process as finish and add its resources to available resources
	 * 6. while (some process finished in the last iteration)
	 * 7. if there is some process which cannot be finish: return SYSERR status
	 * 8. else continue with normal blocking wait call procedure
	 */ 	
	
	intmask mask;			/* Saved interrupt mask		*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

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

	/* waitd logic
	 * detect potential deadlock using resource allocation graph */

	int finish[NPROC];		/* array to hold finish states of processes */
	int work[NSEM];			/* array to hold number workable resources */
	int i, j;

	// update request to detect potential deadlock
	request[getpid()][sem]++;

	// init local work
	for(i = 0; i < NSEM; i++) {
		work[i] = available[i];
	}

	// find processes which are holding some resources
	for(i = 2; i < NPROC; i++) {
		finish[i] = 1;	
		if(isbadpid(i)) continue;

		for(j = 0; j < NSEM; j++) {
			if(allocation[i][j] > 0) {
				//kprintf("\nProc[%d] has resource %d", i, j);
				finish[i] = 0;
				break;
			}
		}
	}

	// try to determine process which can finish safely
	// if not found any processes or all process can finish -> break
	
	while(1) {
		int found = 0;
		for(i = 2; i < NPROC; i++) {
			if(isbadpid(i)) continue;

			if(finish[i] == 1) continue;
		
			// find if all requests of this process that can be satisfied
			int satisfied = 1;
			for(j = 0; j < NSEM; j++) {
				if(request[i][j] > work[j]) {
					//kprintf("\nProcess[%d] requires[%d] %d, available %d", i, j,request[i][j],  work[j]);
					satisfied = 0;
					break;
				}
			}
			// if satisfied update state
			if(satisfied == 1) {
				//kprintf("\nSatisfied %d", i);
				found = 1;
				finish[i] = 1;
				for(j = 0; j < NSEM; j++) {
					work[j] += allocation[i][j];
				}
			}

		}

		// no process could be satisfied in current state
		if(found == 0) break;
	}

	// restore request value
	request[getpid()][sem]--;

	// if any process in cannot finish it is in deadlock
	for(i = 2; i < NPROC; i++) {
		if(isbadpid(i)) continue;
		if(finish[i] == 0) {
			kprintf("\nUnsafe to proceed with process %d!\n", i);	
			restore(mask);
			return SYSERR;
		}
	} 

	//kprintf("\nSafe to proceed!\n");
	
	// allow process to proceed to normal wait call procedure
	if (--(semptr->scount) < 0) {		/* If caller must block	*/
		
		// update request
		request[getpid()][sem]++;

		prptr = &proctab[currpid];
		prptr->prstate = PR_WAIT;	/* Set state to waiting	*/
		prptr->prsem = sem;		/* Record semaphore ID	*/
		enqueue(currpid,semptr->squeue);/* Enqueue on semaphore	*/
		resched();			/*   and reschedule	*/
	} else {
		// update allocation
		allocation[getpid()][sem]++;
		available[sem]--;
	}
	
	restore(mask);
	return OK;	
}



