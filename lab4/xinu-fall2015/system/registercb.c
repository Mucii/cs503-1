/* registercb.c - registercb */

#include <xinu.h>
#include <lab4.h>

/*---------------------------------------------------------------------------
 *  registercb  -  Specify a msg buffer, and also the callback function to be
 *  executed when a message is received.
 *---------------------------------------------------------------------------
 */
syscall registercb(
	umsg32	*buf,	/* Pointer to the message buffer */
	msgcb		cb		/* Callback function to be executed */
	)
{
	/* LAB4TODO */
	return SYSERR;
}
