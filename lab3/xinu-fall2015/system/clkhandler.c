/* clkhandler.c - clkhandler */

#include <xinu.h>
#include <lab2b.h>

/* ayush edit */

/* macro defined to enable bonus mode
 * if BONUS == 1 mode will be on else off
 */

#define BONUS 1

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
  /* LAB2BTODO: Modify this function to add per-process CPU usage monitoring */

	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/

	/* Decrement the ms counter, and see if a second has passed */
	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */
		clktime++;

		/* Reset the local ms counter for the next second */
		count1000 = 1000;
	}

	/* update myglobalclock by 1ms */
	myglobalclock++;

	/* Increase waittime for each process on readylist */
	int i;
	struct procent *ptr;
	for(i = 2; i < NPROC; i++) {
		if(!isbadpid(i)) {
	
			ptr = &proctab[i];
			if(ptr->prstate == PR_READY) {
				ptr->waittime++;

				if(BONUS == 1) {

					/* if the waitseconds is > maxwait at current priority
					 * remove from the current level in multiqueue
					 * insert into the updated priority ts_lwait
					 */

					int waitseconds = ptr->waittime / 1000;
					if(waitseconds > tstab[ptr->prprio].ts_maxwait) {
					
						pid32 pid = getitem(i);
						ptr->prprio = tstab[ptr->prprio].ts_lwait;
						insert(pid, multiqueue[ptr->prprio], ptr->prprio);

					}
				}
			}

		}
	}

	ptr = &proctab[currpid];	

	/* Update the CPU usage time for the current process by 1 ms*/
	if(!isbadpid(currpid)) {
		ptr->prcputime++;
	}
	
	
	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		
		/* if the current process expires its time slice
		 * 	If system process with priority > NUMLEVELS ---> update preempt = QUANTUM
		 * 	else update the priority according to TS dispatch table and update preempt
		 * since process has expired time slice reset waittime = 0
		 */

		if(ptr->prprio < NUMLEVELS) {
			preempt = tstab[ptr->prprio].ts_quantum;
			ptr->prprio = tstab[ptr->prprio].ts_tqexp;
		} else preempt = QUANTUM;
		ptr->waittime = 0;
		resched();
	}
}
