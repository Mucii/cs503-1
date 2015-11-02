/* rt_create.c - rt_create, CS503 15Fall, Lab 3 */

#include <xinu.h>
#include <math.h>
#include <stdarg.h>
local	int newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/* Admission control protocol for RMS
 * TODO: add rmsslk to utilisation
 */

local int admission_control(int rt_period, int rt_comp) {

	uint32	i;			/* Iterate through all processes*/

	double utilisation = 0, maxutilisation;
	
	int k = 0;
	/* Check for all RT valid processes */
	for (i = 1; i < NPROC; i++) {

		if(!isbadpid(i) && proctab[i].prtype == RT_PROC) {
			utilisation += (double)proctab[i].rt_comp / (double) proctab[i].rt_period;
			k++;
		}
	}

	k++;
	utilisation += (double)rt_comp / (double) rt_period;
	// add slack factor
	utilisation += rmsslk;
	maxutilisation = k * (nthroot(2, k) - 1);

	if(utilisation >= maxutilisation) {
		//kprintf("\nAC Test Fail!");
		return SYSERR;
	}
	else 
		return OK;
}

/* Assumption: all RT_PROC will be enqueued at the same level by the scheduler (TS_LEVELS)
 * hence only need to care about within queue priority
 * Computes rt_priority as follows
 * prio = 65535 / tr_period
 * prio = 1 if tr_period > 65535
 */

local pri16 get_rt_priority(int rt_period) {
	int maxperiod = 65535;
	if(rt_period <= 0) return maxperiod;
	if(rt_period >= 65535) return 1;

	else return (maxperiod / rt_period);

}


/*------------------------------------------------------------------------
 *  rt_create  -  Create a real-time process to start running a function on x86
 *------------------------------------------------------------------------
 */

 /* ayush edit */

pid32	rt_create(
	int rt_period,	/* Period */
	int rt_comp,		/* Computation time */
	void *funcaddr,	/* Address of the function	*/
	uint32 ssize,		/* Stack size in words		*/
	char *name,			/* Name (for debugging)		*/
	uint32 nargs,		/* Number of args that follow	*/
	...
	)
{
	uint32		savsp, *pushsp;
	intmask 	mask;    	/* Interrupt mask		*/
	pid32		pid;		/* Stores new process id	*/
	struct	procent	*prptr;		/* Pointer to proc. table entry */
	int32		i;
	uint32		*a;		/* Points to list of args	*/
	uint32		*saddr;		/* Stack address		*/
	pri16		priority;	/* Within RT priority		*/

	mask = disable();
 
 	/* peform admission control
	 * return SYSERR if not possible to accomodate new process
	 */

	if(admission_control(rt_period, rt_comp) == SYSERR)
		return (pid32) SYSERR;

	/* get within RT prioirty */
	priority = get_rt_priority(rt_period);

	//kprintf("\nAC Pass, Prio [%d]", priority);
	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (uint32) roundew(ssize);
	if (((saddr = (uint32 *)getstk(ssize)) ==
	    (uint32 *)SYSERR ) ||
	    (pid=newpid()) == SYSERR || priority < 1 ) {
		restore(mask);
		return SYSERR;
	}

	prcount++;
	prptr = &proctab[pid];

	/* Initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* Initial state is suspended	*/
	prptr->prprio = priority;
	prptr->prstkbase = (char *)saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	prptr->prsem = -1;
	prptr->prparent = (pid32)getpid();
	prptr->prhasmsg = FALSE;

	/* Set up stdin, stdout, and stderr descriptors for the shell	*/
	prptr->prdesc[0] = CONSOLE;
	prptr->prdesc[1] = CONSOLE;
	prptr->prdesc[2] = CONSOLE;

	// added for Lab2B
	prptr->prcputime = 0;

	/* added for LAB3 */

	// Set RT process parameters
	prptr->prtype = RT_PROC;
	prptr->rt_period = rt_period;
	prptr->rt_comp = rt_comp;
	/* Initialize stack as if the process was called		*/

	*saddr = STACKMAGIC;
	savsp = (uint32)saddr;

	/* Push arguments */
	a = (uint32 *)(&nargs + 1);	/* Start of args		*/
	a += nargs -1;			/* Last argument		*/
	for ( ; nargs > 0 ; nargs--)	/* Machine dependent; copy args	*/
		*--saddr = *a--;	/*   onto created process' stack*/
	*--saddr = (long)INITRET;	/* Push on return address	*/

	/* The following entries on the stack must match what ctxsw	*/
	/*   expects a saved process state to contain: ret address,	*/
	/*   ebp, interrupt mask, flags, registerss, and an old SP	*/

	*--saddr = (long)funcaddr;	/* Make the stack look like it's*/
					/*   half-way through a call to	*/
					/*   ctxsw that "returns" to the*/
					/*   new process		*/
	*--saddr = savsp;		/* This will be register ebp	*/
					/*   for process exit		*/
	savsp = (uint32) saddr;		/* Start of frame for ctxsw	*/
	*--saddr = 0x00000200;		/* New process runs with	*/
					/*   interrupts enabled		*/

	/* Basically, the following emulates an x86 "pushal" instruction*/

	*--saddr = 0;			/* %eax */
	*--saddr = 0;			/* %ecx */
	*--saddr = 0;			/* %edx */
	*--saddr = 0;			/* %ebx */
	*--saddr = 0;			/* %esp; value filled in below	*/
	pushsp = saddr;			/* Remember this location	*/
	*--saddr = savsp;		/* %ebp (while finishing ctxsw)	*/
	*--saddr = 0;			/* %esi */
	*--saddr = 0;			/* %edi */
	*pushsp = (unsigned long) (prptr->prstkptr = (char *)saddr);
	restore(mask);
	return pid;
}


/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* Iterate through all processes*/
	// edited for LAB3
	// cannot use static variable since create.c also has a local newpid
	// can make global variable

	pid32 nextpid = 1;		/* Position in table to try or	*/
					/*   one beyond end of table	*/

	/* Check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* Wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}


