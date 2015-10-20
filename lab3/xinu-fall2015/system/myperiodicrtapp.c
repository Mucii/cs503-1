/* myperiodicrtapp.c - myperiodicrtapp */

#include <xinu.h>
#include <lab3.h>

#define LOOP1 20

void myperiodicrtapp(int rt_period, int rt_comp) {
	/* ayush edit */
	int i;
		
	/* vairable names are indicative of their functionalities */
	uint32 period_release, period_start, time_last, time_this, cpu_received, slp_next;

	/* assumed that resume() puts the release time into the unused prsem 
	 * update initial period_release to stored value of prsem */

	period_release = proctab[getpid()].prsem;

	/* arrays to note the important timestamps */
	int release[LOOP1], start[LOOP1], cpu_met[LOOP1];
	

	slp_next = -1;
	for(i = 0; i < LOOP1; i++) {
		// only sleep if there is time left till the next release
		if(slp_next > 0) {
			sleepms(slp_next);
		} else {
			// if entered here means process missed a deadline
			// use debug print below to check if that happens
			//kprintf("PID[%d] missed deadline", getpid());
		}

		// start new period execution time
		period_start = myglobalclock;
		// initialize cpu_received to 0 for current period
		cpu_received = 0;
		
		// initialize time_start
		time_last = myglobalclock;

		release[i] = period_release;
		start[i] = period_start;

		while(1) {
			time_this = myglobalclock;
			uint32 diff = time_this - time_last; 
			
			// update cpu_received if this was still current process
			if(diff < 3) {
				cpu_received += diff;
			} 

			// if computation completed move sleep till next period
			if(cpu_received >= rt_comp) break;
		
			// update the last time
			time_last = time_this;

		}
		
		
		cpu_met[i] = myglobalclock;

		// determine sleep time before next period	
		slp_next = rt_period - (myglobalclock - period_start);
		
		//kprintf("\nPID[%d] sleeptime [%d]", getpid(), slp_next);	
		// update period_release
		period_release += rt_period;			
	}

	/* output the timestamps */
	pid32 pid = getpid();
	for(i = 0; i < LOOP1; i++) 
		kprintf("\nPID: %d, period_release: %d period_start: %d, cpu_met: %d", pid, release[i], start[i], cpu_met[i]);
	kprintf("\n\n");	
}



