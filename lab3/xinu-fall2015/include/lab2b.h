/* lab2b.h - CS503 15Fall, Lab 2 - Part B */

#ifndef __LAB2_B_H_
#define __LAB2_B_H_

void cpuintensive();
void iointensive(uint32 time);
/* Passed in CPU-intensive pid can be used to print that process' CPU usage in between. */
void hybridprocess(pid32 cpu_intensive);

/* LAB2BTODO: Add more declarations here, if necessary. */

#endif  // __LAB2_B_H_
