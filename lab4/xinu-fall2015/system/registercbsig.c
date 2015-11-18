/* registercbsig.c - registercbsig */

#include <xinu.h>
#include <lab4.h>

/*---------------------------------------------------------------------------
 *  registercbsig  -  Specify a signal, and also the callback function to be
 *  executed when the signal is triggered.
 *---------------------------------------------------------------------------
 */
syscall registercbsig(
	uint16	sig,		/* Signal to register */
	sigcb		cb,			/* Callback function to be executed */
	uint32	optarg	/* Optional argument */
	)
{
	/* LAB4TODO */
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
	
	prptr = &proctab[currpid];
	if(sig == ARECV && (cb != NULL && prptr->arecvcb == NULL)) {
		prptr->arecvcb = cb;
		restore(mask);
		return OK;
	}
		
	if(sig == AALRM && (cb != NULL && prptr->alarmcb == NULL && optarg >= 0)) {
		prptr->alarmcb = cb;
		prptr->alrmtime = (myglobalclock + optarg);
		restore(mask);
		return OK;
	}

	restore(mask);
	return SYSERR;
}
