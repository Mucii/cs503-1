/* srpolicy.c - srpolicy */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  srplicy  -  Set the page replacement policy.
 *------------------------------------------------------------------------
 */
syscall srpolicy(int policy)
{
	switch (policy) {
	case FIFO:
		/* LAB5TODO */
		return OK;

	case GCA:
		/* LAB5TODO - Bonus Problem */
		return SYSERR;

	default:
		return SYSERR;
	}
}
