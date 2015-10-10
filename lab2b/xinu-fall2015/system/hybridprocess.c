/* hybridprocess.c - hybridprocess */

#include <xinu.h>
#include <lab2b.h>

void hybridprocess(pid32 cpu_intensive) {
  	/* ayush edit */
	

	struct procent *ptr, *mptr;
	/* If the cpu_intensive process in not running -> exit */

	if(!isbadpid(cpu_intensive))
		ptr = &proctab[cpu_intensive];
	else {
		kprintf("\nBad CPU Process ID %d! Aborting..");
		return;
	}

	/* Method to garner lion's share:
	 * 1. Get current time slice and define usable time as 90% of that.
	 * 2. Run a infinite while loop
	 * 3. Run a big inner loop if cpu_intensive process is still active else exit.
	 * 4. Inside the loop check if usable time exceeded and call sleepms(1)
	 *    Reset the usable time and starttime = myglobalclock
	 */

	mptr = &proctab[getpid()];
	uint32 starttime = myglobalclock, usabletime = tstab[mptr->prprio].ts_quantum;
	usabletime = (uint32)((float)usabletime * 0.9f);
	

	int loop = 100000000;
	while(1) {

		if(isbadpid(cpu_intensive)) {

			kprintf("CPU Process complete..Hybrid exiting!");
			break;
		}
		int j;	
		for(j = 0; j <= loop; j++) {

			uint32 used = myglobalclock - starttime;
			if(used  > usabletime) {
				
				sleepms(1);
				starttime = myglobalclock;
				usabletime = tstab[mptr->prprio].ts_quantum;
				usabletime = (uint32)((float)usabletime * 0.7f);
		
				//kprintf("\nIn Hybrid Prio[%d] cputime [%d], CPUProc Prio[%d] cputime [%d] waittime [%d]", mptr->prprio, mptr->prcputime, ptr->prprio, ptr->prcputime, ptr->waittime);
			}
			
			// Random computations
			int a = 1, b = 2;
			int c = a + b;
			a = c;	
		}

	}

}
