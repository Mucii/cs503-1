/* lab4.h - CS503 15Fall, Lab 4 */

#ifndef __LAB4_H_
#define __LAB4_H_

/** send() with timed blocking. */
syscall	sendb(pid32 pid, umsg32 msg, int32 maxwait);

// for easier usage
typedef int (*msgcb)(umsg32 msg);
typedef int (*sigcb)(void);

/** register a callback function of a message buffer. */
syscall registercb(umsg32 *buf, msgcb cb);

/** register a callback function of signal handling. */
syscall registercbsig(uint16 sig, sigcb cb, uint32 optarg);

/* Signals to be supported in Lab4 */
#define ARECV 10
#define AALRM 11


/* Definitions for the process queue implementation */

/* Number of entries : 1 per process plus 2 per process queue. 
 * We are maintaining a process queue per receiver process */
#define NSQENT (NPROC + NPROC + NPROC)


struct s_qentry {
	qid16 qnext;
	qid16 qprev;
};

extern struct s_qentry sendtab[];


#define	s_firstid(q)	(sendtab[queuehead(q)].qnext)
#define	s_lastid(q)	(sendtab[queuetail(q)].qprev)
#define	s_isempty(q)	(s_firstid(q) >= NPROC)
#define	s_nonempty(q)	(s_firstid(q) <  NPROC)

/* Inline to check queue id assumes interrupts are disabled */

#define	s_isbadqid(x)	(((int32)(x) < 0) || (int32)(x) >= NSQENT-1)


#endif  // __LAB4_H_



