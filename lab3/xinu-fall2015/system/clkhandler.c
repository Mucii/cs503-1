/* clkhandler.c - clkhandler */

#include <xinu.h>
#include <lab2b.h>

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

	/* AYUSH EDIT Lab2b */
	/* Uodate the CPU usage time for the current process by 1 ms*/
	if(!isbadpid(currpid)) {
		struct procent *ptr = &proctab[currpid];
		ptr->prcputime++;
	}
	ctr1000++;
	
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
		preempt = QUANTUM;
		resched();
	}
}
