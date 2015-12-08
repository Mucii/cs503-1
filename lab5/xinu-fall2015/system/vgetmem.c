/* vgetmem.c - vgetmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  vgetmem  -  Allocate heap storage from virtual memory, returning lowest
 *  word address
 *------------------------------------------------------------------------
 */

int startbs() {
	/* write mnext and mlen to first page of backing store
	 * since process has not started page fault cannot work
	 * so that when process starts working it takes correct address */
	
	struct procent *prptr = &proctab[currpid];

	if(prptr->bsstart == 0) return 0;
	
	prptr->bsstart = 0;
	//kprintf("\nPID %d, Writing to Vmemlist->next ", currpid);
	struct memblk *memblock = (struct memblk *) (PNO2VADDR(prptr->vpno));
	memblock->mnext = NULL;
	memblock->mlength = prptr->hsize * NBPG;
	//kprintf("\nPID %d Vmemlist->next initialized", currpid);

	return OK;
}

char  	*vgetmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	/* LAB5TODO */
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	startbs(); 


	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	
	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/

	/* get virtual memory heap list of the current process */

	prev = &(vmemlist[currpid]);
	curr = prev->mnext;

	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			vmemlist[currpid].mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			vmemlist[currpid].mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}

	restore(mask);
	return (char*) SYSERR;
}
