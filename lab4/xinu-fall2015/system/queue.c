/* queue.c - enqueue, dequeue */

#include <xinu.h>

struct qentry	queuetab[NQENT];	/* Table of process queues	*/
struct s_qentry sendtab[NSQENT];	/* Table of send process queues	*/
/* ayush edit 
 * for LAB4 */

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32	enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	int	tail, prev;		/* Tail & previous node indexes	*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	tail = queuetail(q);
	prev = queuetab[tail].qprev;

	queuetab[pid].qnext  = tail;	/* Insert just before tail node	*/
	queuetab[pid].qprev  = prev;
	queuetab[prev].qnext = pid;
	queuetab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (isbadqid(q)) {
		return SYSERR;
	} else if (isempty(q)) {
		return EMPTY;
	}

	pid = getfirst(q);
	queuetab[pid].qprev = EMPTY;
	queuetab[pid].qnext = EMPTY;
	return pid;
}

/*------------------------------------------------------------------------
 * s_enqueue  -  Insert a process at the tail of a send queue
 *------------------------------------------------------------------------
 */

pid32	s_enqueue(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q		/* ID of queue to use		*/
	)
{
	int	tail, prev;		/* Tail & previous node indexes	*/

	if (s_isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}
	//kprintf("\nEnqueing %d to list of %d", pid, q);
	tail = queuetail(q);
	prev = sendtab[tail].qprev;

	sendtab[pid].qnext  = tail;	/* Insert just before tail node	*/
	sendtab[pid].qprev  = prev;
	sendtab[prev].qnext = pid;
	sendtab[tail].qprev = pid;
	return pid;
}

/*------------------------------------------------------------------------
 *  s_dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32	s_dequeue(
	  qid16		q		/* ID queue to use		*/
	)
{
	pid32	pid;			/* ID of process removed	*/

	if (s_isbadqid(q)) {
		return SYSERR;
	} else if (s_isempty(q)) {
		return EMPTY;
	}
	pid = s_getfirst(q);
	//kprintf("\nDequeing %d from list %d", pid, q);
	sendtab[pid].qprev = EMPTY;
	sendtab[pid].qnext = EMPTY;
	return pid;
}
