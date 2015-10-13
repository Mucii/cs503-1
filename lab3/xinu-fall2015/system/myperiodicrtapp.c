/* myperiodicrtapp.c - myperiodicrtapp */

#include <xinu.h>
#include <lab3.h>

#define LOOP1 10

void myperiodicrtapp(int rt_period, int rt_comp) {
	/* LAB3TODO: Emulate an RT application here. */
	int i;
	
	uint32 period_start, time_last, time_this, cpu_received;

	int start[LOOP1], cpu_met[LOOP1];
	
	for(i = 0; i < LOOP1; i++) {

		// start new period
		period_start = myglobalclock;
		cpu_received = 0;
		time_last = myglobalclock;
		start[i] = period_start;

		while(1) {
			time_this = myglobalclock;
			uint32 diff = time_this - time_last; 
			
			// update cpu_received if this was still current process
			if(diff < 3) {
				cpu_received += diff;
			} 
			time_last = myglobalclock;

			if(cpu_received >= rt_comp) break;
		}
		
		// sleep till next period
		cpu_met[i] = myglobalclock;
		uint32 slp_next = rt_period - (myglobalclock - period_start);
		sleepms(slp_next);

	}

	pid32 pid = getpid();
	for(i = 0; i < LOOP1; i++) 
		kprintf("\nPRNAME[%s] PID: %d, period_start: %d, cpu_met: %d", proctab[pid].prname, pid, start[i], cpu_met[i]);
		
}



