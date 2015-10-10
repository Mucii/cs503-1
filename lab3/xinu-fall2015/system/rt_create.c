/* rt_create.c - rt_create, CS503 15Fall, Lab 3 */

#include <xinu.h>
#include <math.h>


/*------------------------------------------------------------------------
 *  rt_create  -  Create a real-time process to start running a function on x86
 *------------------------------------------------------------------------
 */
pid32	rt_create(
	int rt_period,	/* Period */
	int rt_comp,		/* Computation time */
	void *funcaddr,	/* Address of the function	*/
	uint32 ssize,		/* Stack size in words		*/
	char *name,			/* Name (for debugging)		*/
	uint32 nargs,		/* Number of args that follow	*/
	...
	)
{
	/* LAB3TODO: implement rt_create() */
	return (pid32) SYSERR;
}
