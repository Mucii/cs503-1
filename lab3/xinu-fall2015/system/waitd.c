/* waitd.c - waitd */

#include <xinu.h>
#include <lab3.h>


/*------------------------------------------------------------------------
 *  waitd  -  Cause current process to wait on a semaphore.
 *  If a deadlock is resulted, waitd() behaves as a non-blocking call,
 *  with an error status that indicates a DEADLOCK failure.
 *------------------------------------------------------------------------
 */
syscall	waitd(
	sid32		sem		/* semaphore on which to wait  */
	)
{
	/* LAB3TODO: Implement waitd() */
	return SYSERR;
}
