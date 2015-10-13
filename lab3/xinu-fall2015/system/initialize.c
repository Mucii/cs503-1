/* initialize.c - nulluser, sysinit, sizmem */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
extern	void xdone(void);	/* System "shutdown" procedure		*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

// added for Lab2B
struct  ts_ent  tstab[TS_LEVELS];

// added for LAB3
float rmsslk;


/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void	nulluser()
{
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/

	/* Initialize the system */

	sysinit();

	kprintf("\n\r%s\n\n\r", VERSION);

	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\r\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)&text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&text, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&ebss - (uint32)&data);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&data, (uint32)&ebss - 1);

	/* Enable interrupts */

	enable();

	/* Create a process to execute function main() */

	resume (
	   create((void *)main, INITSTK, INITPRIO, "Main process", 0,
           NULL));

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/

	while (TRUE) {
		;		/* Do nothing */
	}

}


// added for Lab2B
void tsinit() {
	int i;

	// initialize ts_tqexp
	for (i = 0; i < TS_LEVELS; i++) {
		if (i == 0) {
			// 0 could only be PRNULL.
			tstab[i].ts_tqexp = 0;
		}
		else if (i <= 10) {
			// Set to 1, so that always > PRNULL.
			tstab[i].ts_tqexp = 1;
		}
		else {
			tstab[i].ts_tqexp = i - 10;
		}
	}

	// initialize ts_slpret
	for (i = 0; i < TS_LEVELS; i++) {
		if (i <= 34) {
			tstab[i].ts_slpret = 50 + i / 10;
		}
		else if (i <= 44) {
			tstab[i].ts_slpret = 54 + (i - 35) / 5;
		}
		else if (i == 45) {
			tstab[i].ts_slpret = 56;
		}
		else if (i == 46) {
			tstab[i].ts_slpret = 57;
		}
		else if (i == 59) {
			tstab[i].ts_slpret = 59;
		}
		else {
			tstab[i].ts_slpret = 58;
		}
	}

	// initialize ts_quantum
	for (i = 0; i < TS_LEVELS; i++) {
		if (i < 40) {
			tstab[i].ts_quantum = 200 - (i / 10) * 40;
		}
		else if (i == 59) {
			tstab[i].ts_quantum = 20;
		}
		else {
			tstab[i].ts_quantum = 40;
		}
	}
}


/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/

	/* Platform Specific Initialization */

	platinit();

	/* Initialize the interrupt vectors */

	initevec();

	/* Initialize free memory list */

	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
		// added for Lab2B
		prptr->prcputime = 0;

		// added for LAB3
		prptr->prtype = TS_PROC;
		prptr->rt_period = -1;
		prptr->rt_comp = -1;
	}

	/* Initialize the Null process entry */

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;

	// Added for LAB3
	prptr->prtype = TS_PROC;
	currpid = NULLPROC;

	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	/* Initialize buffer pools */

	bufinit();

	/* Create a ready list for processes */

	for (i = 0; i <= TS_LEVELS; i++) {
		// if NQENT is not updated correctly, here it may return SYSERR.
		readylists[i] = newqueue();
	}

	rt_readylist = newqueue();
	//readylist = newqueue();

	// added for Lab2B
	tsinit();
	//printtsinfo();

	/* added for LAB3 */
	rmsslk = 0.2;
	
	/* Initialize the real time clock */

	clkinit();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}

	// added for Lab2B
	myglobalclock = 0;

	return;
}

int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}
