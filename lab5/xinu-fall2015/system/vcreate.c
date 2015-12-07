/* vcreate.c - vcreate */

#include <xinu.h>

local	int newpid();
#define	roundew(x)	( (x+3)& ~0x3)


/*----------------------------------------------------------------------------
 *  vcreate  -  Creates a new XINU process. The process's heap will be private
 *  and reside in virtual memory.
 *----------------------------------------------------------------------------
 */
pid32	vcreate(
	  void		*funcaddr,	/* Address of the function	*/
	  uint32	ssize,		/* Stack size in words		*/
		uint32	hsize,		/* Heap size in num of pages */
	  pri16		priority,	/* Process priority > 0		*/
	  char		*name,		/* Name (for debugging)		*/
	  uint32	nargs,		/* Number of args that follow	*/
	  ...
	)
{
	/* code of create */

	uint32		savsp, *pushsp;
	intmask 	mask;    	/* Interrupt mask		*/
	pid32		pid;		/* Stores new process id	*/
	struct	procent	*prptr;		/* Pointer to proc. table entry */
	int32		i;
	uint32		*a;		/* Points to list of args	*/
	uint32		*saddr;		/* Stack address		*/

	mask = disable();
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
	
	/* normal create work done */

	/* VCREATE code */
	
	/* create a new page directory for the process */
	prptr->pd = getpdir();
	if(prptr->pd == NULL) {
		restore(mask);
		return SYSERR;
	
	}
	
	/* check if hsize is more than max allowed */
	if(hsize > numfreebs * MAX_PAGES_PER_BS) {
		kprintf("\nCannot allocated %d heap memory. Aborting!!");
		restore(mask);
		return SYSERR;
	}

	/* update process control block */
	prptr->vpno = VPAGE0;
	prptr->hsize = hsize;
	prptr->bsstart = 1;
	
	uint32 off = 0;
	/* get backing stores */	
	while (hsize > 0) {
		
		uint32 size;
		if(hsize > 200) {
			size = 200;
		} else {
			size = hsize;
		}
		
		hsize -= size;
		bsd_t bsid= allocate_bs(size);
		
		if(bsid == SYSERR) {
			kprintf("\nBacking store cannot be allocated..");
			
			// remove all allocated bs
			remove_bsmappings(pid);
			restore(mask);
			return SYSERR;
		}
		/* Add mapping between this store and process */
		if(add_bsmapping(bsid, pid, prptr->vpno + off, size) == SYSERR) {
			kprintf("\nBacking store cannot be mapped..");
			// remove all allocated bs
			remove_bsmappings(pid);
			restore(mask);
			return SYSERR;
		}
		off += size;
		kprintf("\nPID %d BS %d allocated for process PID %d remaining heap %d.", currpid, bsid, pid, hsize);
	}

	
	/* initiliaze virtual memory heap
	* first item maps to the 1st page in the backing store 
	* it is initialized in first call to vgetmem
	* */
	struct memblk *memptr = &vmemlist[pid];
	memptr->mnext = (struct memblk *) PNO2VADDR(prptr->vpno);
	memptr->mlength = hsize * NBPG;

	restore(mask);
	return pid;

}

// ayush edit
// lab5
local	pid32	newpid(void) {
	uint32	i;			/* Iterate through all processes*/

	pid32 nextpid = 1;
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

