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
	return SYSERR;
}
